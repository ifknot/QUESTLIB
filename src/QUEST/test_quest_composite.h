#ifndef TEST_QUEST_COMPOSITE
#define TEST_QUEST_COMPOSITE

#include "../TDD/tdd_macros.h"

#include "quest_errors.h"
#include "quest_composite.h"

#define QUEST_COMPOSITE_TESTS &test_add_remove_single,  \
                    &test_add_capacity,                 \
                    &test_transfer_all,                 \
                    &test_transfer_type,                \
                    &test_find_component

typedef enum  {     // minimum set of quest game objects
    QUEST_NULL = 0,
    QUEST_LOCATION,
    QUEST_CHARACTER,
    QUEST_PLAYER,
    QUEST_OBJECT,
    QUEST_CONTAINER,
    QUEST_ITEM_SWORD,
    QUEST_CHEST,
    QUEST_ITEM_GOLD,
    QUEST_ITEM_RING,
    QUEST_INVENTORY,
    QUEST_MERCHANT,
    QUEST_ITEM_SCROLL,
    QUEST_ITEM_POTION,
    QUEST_DUNGEON,
    QUEST_ITEM_KEY
} quest_object_t;

// =============================================
// Test Utilities (Arena-based)
// =============================================

static mem_arena_t* test_arena = NULL;

static void test_setup() {
    test_arena = mem_arena_create(MEM_ARENA_POLICY_DOS, MEM_SIZE_16K);
}

static void test_teardown() {
    mem_arena_delete(test_arena);
    test_arena = NULL;
}

static quest_composite_t* create_test_composite(quest_type_t type, const char* brief) {
    quest_info_t* info = quest_info_copy(test_arena, brief, "Test description");
    return quest_composite_create(test_arena, NULL, type, info);
}

static quest_component_t* create_test_item(quest_type_t type, quest_size_t uid, const char* name) {
    quest_info_t* info = quest_info_copy(test_arena, name, "Test item");
    quest_component_t* item = quest_component_create(test_arena, NULL, type, info);
    item->rtti.parts.uid = uid; // Override UID for testing
    return item;
}

// =============================================
// Test Cases
// =============================================

TEST(test_add_remove_single) {
    test_setup();

    quest_composite_t* bag = create_test_composite(QUEST_CONTAINER, "Leather Bag");
    quest_component_t* sword = create_test_item(QUEST_ITEM_SWORD, 1, "Excalibur");
    // Test add
    EXPECT(sword->parent == NULL);
    EXPECT_EQ(quest_composite_add(bag, sword), QUEST_SUCCESS);
    EXPECT_EQ(bag->child_count, 1);
    EXPECT_EQ(sword->parent, (quest_component_t*)bag);
    // Test remove
    EXPECT_EQ(quest_composite_remove(bag, sword), QUEST_SUCCESS);
    EXPECT_EQ(bag->child_count, 0);
    EXPECT(sword->parent == NULL);

    test_teardown();
}

TEST(test_add_capacity) {
    test_setup();
    quest_composite_t* chest = create_test_composite(QUEST_CHEST, "Treasure Chest");

    // Fill to capacity
    for (quest_size_t i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        char name[32];
        snprintf(name, sizeof(name), "Gold Coin %u", i);
        quest_component_t* gold = create_test_item(QUEST_ITEM_GOLD, i, name);
        EXPECT_EQ(quest_composite_add(chest, gold), QUEST_SUCCESS);
    }

    // Test overflow
    quest_component_t* overflow = create_test_item(QUEST_ITEM_RING, 99, "Overflow Ring");
    EXPECT_EQ(quest_composite_add(chest, overflow), QUEST_ITEM_LIST_FULL);

    test_teardown();
}

TEST(test_transfer_all) {
    test_setup();

    quest_composite_t* chest = create_test_composite(QUEST_CHEST, "Dragon's Hoard");
    quest_composite_t* inv = create_test_composite(QUEST_INVENTORY, "Player Inventory");

    // Populate chest
    for (quest_size_t i = 0; i < 5; i++) {
        char name[32];
        snprintf(name, sizeof(name), "Scroll #%u", i);
        quest_component_t* item = create_test_item(QUEST_ITEM_SCROLL, i, name);
        quest_composite_add(chest, item);
    }
    EXPECT_EQ(chest->child_count, 5);
    V(quest_composite_dump(chest, stdout););

    // Test transfer
    EXPECT_EQ(quest_composite_transfer_all(inv, chest), QUEST_SUCCESS);
    EXPECT_EQ(chest->child_count, 0);
    EXPECT_EQ(inv->child_count, 5);

    // Verify parent pointers
    for (quest_size_t i = 0; i < 5; i++) {
        EXPECT_EQ(inv->children[i]->parent, (quest_component_t*)inv);
    }

    test_teardown();
}

TEST(test_transfer_type) {
    test_setup();

    quest_composite_t* merchant = create_test_composite(QUEST_MERCHANT, "Blacksmith");
    quest_composite_t* player = create_test_composite(QUEST_PLAYER, "Hero");

    // Mixed inventory: 3 gold, 2 potions
    quest_component_t* items[5];
    items[0] = create_test_item(QUEST_ITEM_GOLD, 1, "Gold Nugget");
    items[1] = create_test_item(QUEST_ITEM_POTION, 1, "Health Potion");
    items[2] = create_test_item(QUEST_ITEM_GOLD, 2, "Gold Bar");
    items[3] = create_test_item(QUEST_ITEM_POTION, 2, "Mana Potion");
    items[4] = create_test_item(QUEST_ITEM_GOLD, 3, "Gold Coin");

    for (int i = 0; i < 5; i++) {
        quest_composite_add(merchant, items[i]);
    }

    // Test type-filtered transfer
    EXPECT_EQ(quest_composite_transfer_type(player, merchant, QUEST_ITEM_GOLD), QUEST_SUCCESS);
    EXPECT_EQ(merchant->child_count, 2);
    EXPECT_EQ(player->child_count, 3);

    test_teardown();
}

TEST(test_find_component) {
    test_setup();

    quest_composite_t* dungeon = create_test_composite(QUEST_DUNGEON, "Dark Crypt");
    quest_component_t* key = create_test_item(QUEST_ITEM_KEY, 42, "Rusty Key");
    quest_composite_add(dungeon, key);

    // Test find
    quest_fingerprint_t target = key->rtti.fingerprint;
    EXPECT_EQ(quest_composite_find(dungeon, target), key);

    // Test not found
    quest_rtti_t fake;
    fake.parts.type = QUEST_ITEM_KEY;
    fake.parts.uid = 99;
    EXPECT_EQ(quest_composite_find(dungeon, fake.fingerprint), NULL);

    test_teardown();
}

#endif

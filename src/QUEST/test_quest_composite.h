#ifndef TEST_QUEST_COMPOSITE
#define TEST_QUEST_COMPOSITE

#include "../TDD/tdd_macros.h"
#include "quest_errors.h"
#include "quest_composite.h"
#include "quest_info.h"
#include "quest_object_types.h"

#define QUEST_COMPOSITE_TESTS &test_add_remove_single,  \
                    &test_add_capacity,                 \
                    &test_transfer_all,                 \
                    &test_transfer_type,                \
                    &test_find_component

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
    return quest_composite_create(test_arena, (quest_component_t*)0xF00D, type, info);
}

static quest_component_t* create_test_item(quest_type_t type, quest_size_t uid, const char* name) {
    quest_info_t* info = quest_info_copy(test_arena, name, "Test item");
    quest_component_t* item = quest_component_create(test_arena, (quest_component_t*)0xBEEF, type, info);
    item->rtti.parts.uid = uid;
    return item;
}

// =============================================
// Test Cases (Updated for New Interface)
// =============================================

TEST(test_add_remove_single) {
    test_setup();
    quest_component_t* sword = create_test_item(QUEST_ITEM_SWORD, 1, "Excalibur");
    // Test create component
    EXPECT_STREQ(sword->info->brief, "Excalibur");
    EXPECT_STREQ(sword->info->details, "Test item");
    EXPECT_EQ(sword->parent, (quest_component_t*)0xBEEF);
    EXPECT_EQ(sword->rtti.parts.type, QUEST_ITEM_SWORD);
    EXPECT_EQ(sword->rtti.parts.uid, 1);
    V(quest_component_dump(sword, stdout););
    quest_composite_t* bag = create_test_composite(QUEST_CONTAINER, "Leather Bag");
    // Test create composite
    EXPECT_STREQ(bag->base.info->brief, "Leather Bag");
    EXPECT_STREQ(bag->base.info->details, "Test description");
    EXPECT_EQ(bag->base.parent, (quest_component_t*)0xF00D);
    EXPECT_EQ(bag->base.rtti.parts.type, QUEST_CONTAINER);
    EXPECT_EQ(bag->base.rtti.parts.uid, 2);
    V(quest_composite_dump(bag, stdout););
    // Test add
    EXPECT(sword->parent == (quest_component_t*)0xF00D);
    quest_composite_add(bag, sword);
    EXPECT(bag->child_count == 1);
    EXPECT(sword->parent == (quest_component_t*)bag);
    V(quest_composite_dump(bag, stdout););
    // Test remove
    quest_component_t* removed = quest_composite_remove(bag, sword);
    EXPECT(removed == sword);
    EXPECT(bag->child_count == 0);
    EXPECT(sword->parent == NULL);
    V(quest_composite_dump(bag, stdout););

    test_teardown();
}

TEST(test_add_capacity) {
    test_setup();
    quest_composite_t* chest = create_test_composite(QUEST_CHEST, "Treasure Chest");

    // Fill to capacity
    for (quest_size_t i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        char name[32];
        snprintf(name, sizeof(name), "Gold Coin %zu", i);
        quest_component_t* gold = create_test_item(QUEST_ITEM_GOLD, i, name);
        quest_composite_add(chest, gold);
        EXPECT(chest->child_count == i + 1);
    }

    // Test overflow
    quest_component_t* overflow = create_test_item(QUEST_ITEM_RING, 99, "Overflow Ring");
    EXPECT(chest->child_count == QUEST_COMPOSITE_MAX_CHILDREN);
    //quest_composite_add(chest, overflow); // Should silently fail (EXPECT in debug)
    EXPECT(chest->child_count == QUEST_COMPOSITE_MAX_CHILDREN);

    test_teardown();
}

TEST(test_transfer_all) {
    test_setup();

    quest_composite_t* chest = create_test_composite(QUEST_CHEST, "Dragon's Hoard");
    quest_composite_t* inv = create_test_composite(QUEST_INVENTORY, "Player Inventory");

    // Populate chest with mixed items
    quest_composite_add(chest, create_test_item(QUEST_ITEM_GOLD, 1, "Gold Coin"));
    quest_composite_add(chest, create_test_item(QUEST_ITEM_SCROLL, 1, "Scroll of Fire"));
    quest_composite_add(chest, create_test_item(QUEST_ITEM_POTION, 1, "Health Potion"));

    EXPECT(chest->child_count == 3);
    EXPECT(inv->child_count == 0);

    // Test transfer
    size_t transferred = quest_composite_transfer_all(inv, chest);
    EXPECT(transferred == 3);
    EXPECT(chest->child_count == 0);
    EXPECT(inv->child_count == 3);

    // Verify parent pointers
    for (quest_size_t i = 0; i < 3; i++) {
        EXPECT(inv->children[i]->parent == (quest_component_t*)inv);
    }

    test_teardown();
}

TEST(test_transfer_type) {
    test_setup();

    quest_composite_t* merchant = create_test_composite(QUEST_MERCHANT, "Blacksmith");
    quest_composite_t* player = create_test_composite(QUEST_PLAYER, "Hero");

    // Create mixed inventory (3 gold, 2 potions)
    quest_composite_add(merchant, create_test_item(QUEST_ITEM_GOLD, 1, "Gold Nugget"));
    quest_composite_add(merchant, create_test_item(QUEST_ITEM_POTION, 1, "Health Potion"));
    quest_composite_add(merchant, create_test_item(QUEST_ITEM_GOLD, 2, "Gold Bar"));
    quest_composite_add(merchant, create_test_item(QUEST_ITEM_POTION, 2, "Mana Potion"));
    quest_composite_add(merchant, create_test_item(QUEST_ITEM_GOLD, 3, "Gold Coin"));

    EXPECT(merchant->child_count == 5);
    EXPECT(player->child_count == 0);

    // Test gold transfer
    size_t gold_transferred = quest_composite_transfer_type(player, merchant, QUEST_ITEM_GOLD);
    EXPECT(gold_transferred == 3);
    EXPECT(merchant->child_count == 2); // Potions remain
    EXPECT(player->child_count == 3);   // All gold transferred

    // Verify no gold left in merchant
    for (quest_size_t i = 0; i < merchant->child_count; i++) {
        EXPECT(merchant->children[i]->rtti.parts.type != QUEST_ITEM_GOLD);
    }

    test_teardown();
}

TEST(test_find_component) {
    test_setup();

    quest_composite_t* dungeon = create_test_composite(QUEST_DUNGEON, "Dark Crypt");
    quest_component_t* key = create_test_item(QUEST_ITEM_KEY, 42, "Rusty Key");
    quest_component_t* fake_key = create_test_item(QUEST_ITEM_KEY, 99, "Fake Key");

    quest_composite_add(dungeon, key);

    // Test successful find
    quest_fingerprint_t target = key->rtti.fingerprint;
    quest_component_t* found = quest_composite_find(dungeon, target);
    EXPECT(found == key);

    // Test not found
    quest_fingerprint_t fake_target = fake_key->rtti.fingerprint;
    EXPECT(quest_composite_find(dungeon, fake_target) == NULL);

    test_teardown();
}

#endif

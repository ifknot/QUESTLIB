#ifndef TEST_QUEST_COMPOSITE
#define TEST_QUEST_COMPOSITE

#include "../TDD/tdd_macros.h"
#include "quest_errors.h"
#include "quest_composite.h"
#include "quest_info.h"
#include "quest_object_types.h"
#include "quest_features.h"

#define QUEST_COMPOSITE_TESTS \
    &test_component_creation,    \
    &test_composite_creation,    \
    &test_add_remove_single,     \
    &test_add_capacity,          \
    &test_transfer_all,          \
    &test_transfer_type,         \
    &test_find_component,        \
    &test_count_type,            \
    &test_find_type,             \
    &test_enumerate_type,        \
    &test_feature_set_clear_single, \
    &test_feature_bulk_operations, \
    &test_feature_queries,      \
    &test_feature_edge_cases

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
// Test Cases (Complete Interface Coverage)
// =============================================

TEST(test_component_creation) {
    test_setup();

    // Test basic component creation
    quest_component_t* potion = create_test_item(QUEST_POTION, 1, "Healing Potion");
    EXPECT_NOT_NULL(potion);
    EXPECT_STREQ(potion->info->brief, "Healing Potion");
    EXPECT_EQ(potion->rtti.parts.type, QUEST_POTION);
    EXPECT_EQ(potion->rtti.parts.uid, 1);
    EXPECT_EQ(potion->parent, (quest_component_t*)0xBEEF);

    // Test component initialization
    quest_component_t custom_comp;
    quest_info_t* custom_info = quest_info_copy(test_arena, "Custom", "Custom component");
    quest_component_init(&custom_comp, NULL, QUEST_ARMOR, custom_info);
    EXPECT_STREQ(custom_comp.info->brief, "Custom");
    EXPECT_EQ(custom_comp.parent, NULL);
    EXPECT_EQ(custom_comp.rtti.parts.type, QUEST_ARMOR);

    test_teardown();
}

TEST(test_composite_creation) {
    test_setup();

    // Test basic composite creation
    quest_composite_t* room = create_test_composite(QUEST_ROOM, "Dungeon Cell");
    EXPECT_NOT_NULL(room);
    EXPECT_STREQ(room->base.info->brief, "Dungeon Cell");
    EXPECT_EQ(room->base.rtti.parts.type, QUEST_ROOM);
    EXPECT_EQ(room->child_count, 0);

    // Test composite initialization
    quest_composite_t custom_comp;
    quest_info_t* custom_info = quest_info_copy(test_arena, "Custom", "Custom composite");
    quest_composite_init(&custom_comp, NULL, QUEST_CONTAINER, custom_info);
    EXPECT_STREQ(custom_comp.base.info->brief, "Custom");
    EXPECT_EQ(custom_comp.base.parent, NULL);
    EXPECT_EQ(custom_comp.child_count, 0);

    test_teardown();
}

TEST(test_add_remove_single) {
    test_setup();
    quest_component_t* sword = create_test_item(QUEST_SWORD, 1, "Excalibur");
    quest_composite_t* bag = create_test_composite(QUEST_CONTAINER, "Leather Bag");

    // Test add
    quest_composite_add(bag, sword);
    EXPECT_EQ(bag->child_count, 1);
    EXPECT_EQ(sword->parent, (quest_component_t*)bag);
    EXPECT_EQ(bag->children[0], sword);

    // Test remove
    quest_component_t* removed = quest_composite_remove(bag, sword);
    EXPECT_EQ(removed, sword);
    EXPECT_EQ(bag->child_count, 0);
    EXPECT_EQ(sword->parent, NULL);
    EXPECT_EQ(bag->children[0], NULL);

    test_teardown();
}

TEST(test_add_capacity) {
    test_setup();
    quest_composite_t* chest = create_test_composite(QUEST_CHEST, "Treasure Chest");

    // Fill to capacity
    for (quest_size_t i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        char name[32];
        snprintf(name, sizeof(name), "Gold Coin %zu", i);
        quest_component_t* gold = create_test_item(QUEST_GOLD, i, name);
        quest_composite_add(chest, gold);
        EXPECT_EQ(chest->child_count, i + 1);
    }

    // Test overflow protection
    //quest_component_t* overflow = create_test_item(QUEST_RING, 99, "Overflow Ring");
    //quest_composite_add(chest, overflow); // Should be rejected - it is
    //EXPECT_EQ(chest->child_count, QUEST_COMPOSITE_MAX_CHILDREN);

    test_teardown();
}

TEST(test_transfer_all) {
    test_setup();
    quest_composite_t* chest = create_test_composite(QUEST_CHEST, "Dragon's Hoard");
    quest_composite_t* inv = create_test_composite(QUEST_INVENTORY, "Player Inventory");

    // Populate chest
    quest_composite_add(chest, create_test_item(QUEST_GOLD, 1, "Gold Coin"));
    quest_composite_add(chest, create_test_item(QUEST_SCROLL, 1, "Scroll of Fire"));
    quest_composite_add(chest, create_test_item(QUEST_POTION, 1, "Health Potion"));

    // Test full transfer
    size_t transferred = quest_composite_transfer_all(inv, chest);
    EXPECT_EQ(transferred, 3);
    EXPECT_EQ(chest->child_count, 0);
    EXPECT_EQ(inv->child_count, 3);

    // Verify parent pointers updated
    for (quest_size_t i = 0; i < 3; i++) {
        EXPECT_EQ(inv->children[i]->parent, (quest_component_t*)inv);
    }

    test_teardown();
}

TEST(test_transfer_type) {
    test_setup();
    quest_composite_t* merchant = create_test_composite(QUEST_MERCHANT, "Blacksmith");
    quest_composite_t* player = create_test_composite(QUEST_PLAYER, "Hero");

    // Create mixed inventory
    quest_composite_add(merchant, create_test_item(QUEST_GOLD, 1, "Gold Nugget"));
    quest_composite_add(merchant, create_test_item(QUEST_POTION, 1, "Health Potion"));
    quest_composite_add(merchant, create_test_item(QUEST_GOLD, 2, "Gold Bar"));
    quest_composite_add(merchant, create_test_item(QUEST_POTION, 2, "Mana Potion"));
    quest_composite_add(merchant, create_test_item(QUEST_GOLD, 3, "Gold Coin"));

    // Test selective transfer
    size_t gold_transferred = quest_composite_transfer_type(player, merchant, QUEST_GOLD);
    EXPECT_EQ(gold_transferred, 3);
    EXPECT_EQ(merchant->child_count, 2); // Potions remain
    EXPECT_EQ(player->child_count, 3);   // Gold transferred

    // Verify only gold was transferred
    for (quest_size_t i = 0; i < player->child_count; i++) {
        EXPECT_EQ(player->children[i]->rtti.parts.type, QUEST_GOLD);
    }

    test_teardown();
}

TEST(test_find_component) {
    test_setup();
    quest_composite_t* dungeon = create_test_composite(QUEST_DUNGEON, "Dark Crypt");
    quest_component_t* key = create_test_item(QUEST_KEY, 42, "Rusty Key");
    quest_component_t* fake_key = create_test_item(QUEST_KEY, 99, "Fake Key");

    quest_composite_add(dungeon, key);

    // Test successful find
    quest_component_t* found = quest_composite_find_fingerprint(dungeon, key->rtti.fingerprint);
    EXPECT_EQ(found, key);

    // Test not found case
    EXPECT_NULL(quest_composite_find_fingerprint(dungeon, fake_key->rtti.fingerprint));

    test_teardown();
}

TEST(test_count_type) {
    test_setup();
    quest_composite_t* room = create_test_composite(QUEST_ROOM, "Armory");

    // Add mixed items
    quest_composite_add(room, create_test_item(QUEST_SWORD, 1, "Longsword"));
    quest_composite_add(room, create_test_item(QUEST_SWORD, 2, "Shortsword"));
    quest_composite_add(room, create_test_item(QUEST_SHIELD, 1, "Wooden Shield"));
    quest_composite_add(room, create_test_item(QUEST_SWORD, 3, "Broadsword"));

    // Test type counting
    EXPECT_EQ(quest_composite_count_type(room, QUEST_SWORD), 3);
    EXPECT_EQ(quest_composite_count_type(room, QUEST_SHIELD), 1);
    EXPECT_EQ(quest_composite_count_type(room, QUEST_POTION), 0);

    test_teardown();
}

TEST(test_find_type) {
    test_setup();
    quest_composite_t* tavern = create_test_composite(QUEST_TAVERN, "Dragon's Inn");

    // Add NPCs and items
    quest_composite_add(tavern, create_test_item(QUEST_NPC, 1, "Barkeep"));
    quest_composite_add(tavern, create_test_item(QUEST_CHAIR, 1, "Wooden Chair"));
    quest_composite_add(tavern, create_test_item(QUEST_NPC, 2, "Bard"));
    quest_composite_add(tavern, create_test_item(QUEST_TABLE, 1, "Round Table"));

    // Test finding first of type
    quest_component_t* first_npc = quest_composite_find_type(tavern, QUEST_NPC);
    EXPECT_NOT_NULL(first_npc);
    EXPECT_STREQ(first_npc->info->brief, "Barkeep");

    quest_component_t* first_table = quest_composite_find_type(tavern, QUEST_TABLE);
    EXPECT_NOT_NULL(first_table);
    EXPECT_STREQ(first_table->info->brief, "Round Table");

    // Test not found case
    EXPECT_NULL(quest_composite_find_type(tavern, QUEST_POTION));

    test_teardown();
}

TEST(test_enumerate_type) {
    test_setup();
    quest_composite_t* library = create_test_composite(QUEST_LIBRARY, "Great Library");

    // Add books and other items
    quest_composite_add(library, create_test_item(QUEST_BOOK, 1, "History of Magic"));
    quest_composite_add(library, create_test_item(QUEST_SCROLL, 1, "Scroll of Recall"));
    quest_composite_add(library, create_test_item(QUEST_BOOK, 2, "Bestiary"));
    quest_composite_add(library, create_test_item(QUEST_BOOK, 3, "Alchemy Manual"));
    quest_composite_add(library, create_test_item(QUEST_SCROLL, 2, "Scroll of Fire"));

    // Prepare storage
    quest_component_t* books[10];
    quest_size_t book_count = quest_composite_enumerate_type(library, QUEST_BOOK, books);

    // Verify results
    EXPECT_EQ(book_count, 3);
    EXPECT_STREQ(books[0]->info->brief, "History of Magic");
    EXPECT_STREQ(books[1]->info->brief, "Bestiary");
    EXPECT_STREQ(books[2]->info->brief, "Alchemy Manual");

    // Test empty result
    quest_component_t* potions[10];
    quest_size_t potion_count = quest_composite_enumerate_type(library, QUEST_POTION, potions);
    EXPECT_EQ(potion_count, 0);

    test_teardown();
}

TEST(test_feature_set_clear_single) {
    quest_component_t comp = {0};

    // Set single feature
    quest_component_set_features(&comp, COMP_FEATURE_VISIBLE);
    EXPECT(quest_component_has_all_features(&comp, COMP_FEATURE_VISIBLE));
    EXPECT_EQ(comp.features, COMP_FEATURE_VISIBLE);

    // Clear single feature
    quest_component_clear_features(&comp, COMP_FEATURE_VISIBLE);
    EXPECT(!quest_component_has_any_features(&comp, COMP_FEATURE_VISIBLE));
    EXPECT_EQ(comp.features, 0);
}

TEST(test_feature_bulk_operations) {
    quest_component_t comp = {0};
    const quest_bitmask_t mask = COMP_FEATURE_VISIBLE | COMP_FEATURE_INTERACTABLE;

    // Set multiple
    quest_component_set_features(&comp, mask);
    EXPECT(quest_component_has_all_features(&comp, mask));
    EXPECT_EQ(comp.features, mask);

    // Clear one
    quest_component_clear_features(&comp, COMP_FEATURE_VISIBLE);
    EXPECT(quest_component_has_all_features(&comp, COMP_FEATURE_INTERACTABLE));
    EXPECT(!quest_component_has_any_features(&comp, COMP_FEATURE_VISIBLE));
}

TEST(test_feature_queries) {
    quest_component_t comp = {0};
    comp.features = COMP_FEATURE_VISIBLE | COMP_FEATURE_INTERACTABLE;

    // has_all (AND)
    EXPECT(quest_component_has_all_features(&comp,
        COMP_FEATURE_VISIBLE | COMP_FEATURE_INTERACTABLE));
    EXPECT(!quest_component_has_all_features(&comp,
        COMP_FEATURE_VISIBLE | COMP_FEATURE_PERSISTENT));

    // has_any (OR)
    EXPECT(quest_component_has_any_features(&comp,
        COMP_FEATURE_VISIBLE | COMP_FEATURE_PERSISTENT));
    EXPECT(!quest_component_has_any_features(&comp,
        COMP_FEATURE_PERSISTENT));
}

TEST(test_feature_edge_cases) {
    quest_component_t comp = {0};

    // Zero input (should be no-op)
    quest_component_set_features(&comp, 0);
    EXPECT_EQ(comp.features, 0);

    quest_component_clear_features(&comp, 0);
    EXPECT_EQ(comp.features, 0);

    // Invalid bits (should ignore)
    quest_component_set_features(&comp, 0x80000000);
    EXPECT_EQ(comp.features, 0);
    /*
    #ifndef NDEBUG
    printf("Expect assertion failures:\n");
    EXPECT_ASSERT(quest_component_set_features(NULL, COMP_FEATURE_VISIBLE));
    EXPECT_ASSERT(quest_component_has_all_features(NULL, COMP_FEATURE_VISIBLE));
    #endif
    */
}


#endif

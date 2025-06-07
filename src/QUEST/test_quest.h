#ifndef TEST_QUEST_H
#define TEST_QUEST_H

#include "../TDD/tdd_macros.h"
#include "../TDD/tdd_progress.h"
//#include "../MEM/mem_arena.h"

#include "quest_uid.h"
#include "quest_rtti.h"
#include "quest_composite.h"

#define QUEST_TESTS &test_uid,                      \
                    &test_rtti_creation,            \
                    &test_type_checks,              \
                    &test_uid_uniqueness,           \
                    &test_fingerprint_components,   \
                    &test_null_type_handling,       \
                    &test_add_remove_single,        \
                    &test_add_capacity,             \
                    &test_transfer_all,             \
                    &test_transfer_type,            \
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
// UID and RTTI
// =============================================

TEST(test_uid) {
    const uint32_t ITERATIONS = 1000;
    tdd_progress_t prg = tdd_progress_make(ITERATIONS, 0, 0, 30); // 30-character wide bar starting at zero and calculating steps
    for (uint32_t i = 1; i <= ITERATIONS; i++) {
        tdd_progress_bar(&prg);
        ASSERT(i == quest_next_uid());
    }
}

TEST(test_rtti_creation) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_PLAYER);

    EXPECT_EQ(quest_rtti_type(rtti), QUEST_PLAYER);
    EXPECT_GT(quest_rtti_uid(rtti), 0);  // UIDs start at 1
}

TEST(test_type_checks) {
    quest_rtti_t loc = quest_rtti_create(QUEST_LOCATION);
    quest_rtti_t obj = quest_rtti_create(QUEST_OBJECT);

    EXPECT_EQ(quest_is_typeof(loc, QUEST_LOCATION), true);
    EXPECT_EQ(quest_is_typeof(obj, QUEST_OBJECT), true);
    EXPECT_EQ(quest_is_typeof(loc, QUEST_OBJECT), false);
}

TEST(test_uid_uniqueness) {
    quest_rtti_t rtti1 = quest_rtti_create(QUEST_CHARACTER);
    quest_rtti_t rtti2 = quest_rtti_create(QUEST_CHARACTER);

    EXPECT_EQ(quest_rtti_type(rtti1), quest_rtti_type(rtti2));
    EXPECT_NEQ(quest_rtti_uid(rtti1), quest_rtti_uid(rtti2));
    EXPECT_NEQ(quest_rtti_fingerprint(rtti1), quest_rtti_fingerprint(rtti2));
}

TEST(test_fingerprint_components) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_OBJECT);
    quest_fingerprint_t fp = quest_rtti_fingerprint(rtti);

    // Verify bit layout matches documentation
    EXPECT_EQ((fp >> 16) & 0xFFFF, quest_rtti_type(rtti));
    EXPECT_EQ(fp & 0xFFFF, quest_rtti_uid(rtti));
}

TEST(test_null_type_handling) {
    quest_rtti_t rtti = quest_rtti_create(QUEST_NULL);

    EXPECT_EQ(quest_rtti_type(rtti), QUEST_NULL);
    EXPECT_GT(quest_rtti_uid(rtti), 0);  // UID still valid for NULL type
}

// =============================================
// Composite
// =============================================

static quest_composite_t* create_test_composite(quest_type_t type) {
    quest_composite_t* comp = (quest_composite_t*)malloc(sizeof(quest_composite_t));
    comp->base.rtti = quest_rtti_create(type);
    comp->base.parent = NULL;
    comp->child_count = 0;
    return comp;
}

static quest_component_t* create_test_item(quest_type_t type, quest_size_t uid) {
    quest_component_t* item = (quest_component_t*)malloc(sizeof(quest_component_t));
    item->rtti.parts.type = type;
    item->rtti.parts.uid = uid;
    item->parent = NULL;
    return item;
}

// =============================================
// Test Cases
// =============================================

TEST(test_add_remove_single) {
    quest_composite_t* bag = create_test_composite(QUEST_CONTAINER);
    quest_component_t* sword = create_test_item(QUEST_ITEM_SWORD, 1);

    // Test successful add
    EXPECT_EQ(quest_composite_add(bag, sword), QUEST_SUCCESS);
    EXPECT_EQ(bag->child_count, 1);
    //EXPECT_EQ(sword->parent, (quest_component_t*)bag);

    // Test successful remove
    quest_fingerprint_t target = sword->rtti.fingerprint;
    EXPECT_EQ(quest_composite_remove(bag, target), QUEST_SUCCESS);
    EXPECT_EQ(bag->child_count, 0);
    //EXPECT_EQ(sword->parent, );

    free(sword);
    free(bag);
}

TEST(test_add_capacity) {
    quest_composite_t* chest = create_test_composite(QUEST_CHEST);

    // Fill to capacity
    for (quest_size_t i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        quest_component_t* gold = create_test_item(QUEST_ITEM_GOLD, i);
        EXPECT_EQ(quest_composite_add(chest, gold), QUEST_SUCCESS);
    }

    // Test overflow
    quest_component_t* overflow = create_test_item(QUEST_ITEM_RING, 99);
    EXPECT_EQ(quest_composite_add(chest, overflow), QUEST_ITEM_LIST_FULL);

    free(overflow);
    for (quest_size_t i = 0; i < QUEST_COMPOSITE_MAX_CHILDREN; i++) {
        free(chest->children[i]);
    }
    free(chest);
}

TEST(test_transfer_all) {
    quest_composite_t* chest = create_test_composite(QUEST_CHEST);
    quest_composite_t* inventory = create_test_composite(QUEST_INVENTORY);

    // Populate chest
    for (quest_size_t i = 0; i < 5; i++) {
        quest_component_t* item = create_test_item(QUEST_ITEM_SCROLL, i);
        quest_composite_add(chest, item);
    }

    // Test full transfer
    EXPECT_EQ(quest_composite_transfer_all(chest, inventory), QUEST_SUCCESS);
    EXPECT_EQ(chest->child_count, 0);
    EXPECT_EQ(inventory->child_count, 5);

    // Verify parent pointers
    for (quest_size_t i = 0; i < 5; i++) {
        //EXPECT_EQ(inventory->children[i]->parent, (quest_component_t*)inventory);
    }

    for (quest_size_t i = 0; i < 5; i++) {
        free(inventory->children[i]);
    }
    free(chest);
    free(inventory);
}

TEST(test_transfer_type) {
    quest_composite_t* merchant = create_test_composite(QUEST_MERCHANT);
    quest_composite_t* player = create_test_composite(QUEST_PLAYER);

    // Mixed inventory: 3 gold, 2 potions
    quest_component_t* items[5];
    items[0] = create_test_item(QUEST_ITEM_GOLD, 1);
    items[1] = create_test_item(QUEST_ITEM_POTION, 1);
    items[2] = create_test_item(QUEST_ITEM_GOLD, 2);
    items[3] = create_test_item(QUEST_ITEM_POTION, 2);
    items[4] = create_test_item(QUEST_ITEM_GOLD, 3);

    for (int i = 0; i < 5; i++) {
        quest_composite_add(merchant, items[i]);
    }

    // Test type-filtered transfer
    quest_size_t transferred = quest_composite_transfer_type(
        merchant, player, QUEST_ITEM_GOLD
    );

    EXPECT_EQ(transferred, 3);
    EXPECT_EQ(merchant->child_count, 2);
    EXPECT_EQ(player->child_count, 3);

    // Cleanup
    for (int i = 0; i < merchant->child_count; i++) {
        free(merchant->children[i]);
    }
    for (int i = 0; i < player->child_count; i++) {
        free(player->children[i]);
    }

    free(merchant);
    free(player);
}

TEST(test_find_component) {
    quest_composite_t* dungeon = create_test_composite(QUEST_DUNGEON);

    quest_rtti_t target;
    target.parts.type = QUEST_ITEM_KEY;
    target.parts.uid = 42;
    quest_component_t* key = create_test_item(target.parts.type, target.parts.uid);
    quest_composite_add(dungeon, key);

    // Test successful find
    quest_component_t* found = quest_composite_find(dungeon, target.fingerprint);
    EXPECT_EQ(found, key);

    // Test not found
    quest_rtti_t fake = { .parts.type = QUEST_ITEM_KEY, .parts.uid = 99};
    EXPECT_EQ(quest_composite_find(dungeon, fake.fingerprint), NULL);

    free(key);
    free(dungeon);
}

#endif

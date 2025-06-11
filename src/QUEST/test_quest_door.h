#ifndef TEST_QUEST_DOOR_H
#define TEST_QUEST_DOOR_H

#include "../TDD/tdd_macros.h"
#include "../MEM/mem_arena.h"
#include "quest_door.h"
#include "quest_location.h"

// =============================================
// Test Configuration
// =============================================
#define QUEST_DOOR_TESTS &test_door_creation,     \
                         &test_door_unlocking,    \
                         &test_door_locking,      \
                         &test_door_edge_cases

static mem_arena_t* test_arena = NULL;
static quest_rtti_t test_key = { .parts = {QUEST_ITEM_KEY, 42} };
static quest_rtti_t wrong_key = { .parts = {QUEST_ITEM_KEY, 99} };

// =============================================
// Test Utilities
// =============================================
static void setup() {
    test_arena = mem_arena_create(MEM_ARENA_POLICY_DOS, MEM_SIZE_2K);
    ASSERT(test_arena != NULL, "Arena creation failed");
    V(printf("Test setup: Arena initialized\n"));
}

static void teardown() {
    if (test_arena) {
        mem_arena_delete(test_arena);
        test_arena = NULL;
    }
    V(printf("Test teardown: Arena cleaned\n\n"));
}

static quest_location_t* create_test_location(char symbol) {
    quest_info_t* info = quest_info_create(test_arena, "Test Loc", "Test location");
    quest_location_t* loc = quest_location_create(test_arena, NULL, QUEST_LOCATION, info, symbol);
    ASSERT(loc != NULL, "Location creation failed");
    return loc;
}

// =============================================
// Test Cases
// =============================================
TEST(test_door_creation) {
    setup();

    // Create test locations
    quest_location_t* loc1 = create_test_location('A');
    quest_location_t* loc2 = create_test_location('B');
    
    // Create door with key requirement
    quest_info_t* door_info = quest_info_create(test_arena, "Oak Door", "Sturdy wooden door");
    quest_door_t* door = quest_door_create(test_arena, loc1, loc2, 
                                         QUEST_DOOR_WOODEN, door_info, true, test_key);
    
    // Verify creation
    EXPECT(door != NULL);
    EXPECT_EQ(door->base.base.rtti.parts.type, QUEST_DOOR_WOODEN);
    EXPECT(door->is_locked);
    EXPECT_EQ(door->key.fingerprint, test_key.fingerprint);
    EXPECT_EQ(door->base.locations[0], loc1);
    EXPECT_EQ(door->base.locations[1], loc2);
    
    V(printf("Created door between locations A and B\n"));

    teardown();
}

TEST(test_door_unlocking) {
    setup();

    // Create locked door
    quest_door_t* door = quest_door_create(test_arena, NULL, NULL, 
                                         QUEST_DOOR_WOODEN, NULL, true, test_key);
    ASSERT(door != NULL, "Door creation failed");
    
    // Test wrong key
    EXPECT_EQ(quest_door_unlock(door, wrong_key), QUEST_WRONG_KEY);
    EXPECT(door->is_locked);
    V(printf("Rejected wrong key (ID:99)\n"));
    
    // Test correct key
    EXPECT_EQ(quest_door_unlock(door, test_key), QUEST_SUCCESS);
    EXPECT(!door->is_locked);
    V(printf("Unlocked with correct key (ID:42)\n"));
    
    // Test already unlocked
    EXPECT_EQ(quest_door_unlock(door, test_key), QUEST_ALREADY_UNLOCKED);
    V(printf("Properly handled already-unlocked state\n"));

    teardown();
}

TEST(test_door_locking) {
    setup();

    // Create unlocked door
    quest_door_t* door = quest_door_create(test_arena, NULL, NULL, 
                                         QUEST_DOOR_WOODEN, NULL, false, test_key);
    ASSERT(door != NULL, "Door creation failed");
    
    // Test initial lock
    EXPECT_EQ(quest_door_lock(door), QUEST_SUCCESS);
    EXPECT(door->is_locked);
    V(printf("Successfully locked door\n"));
    
    // Test already locked
    EXPECT_EQ(quest_door_lock(door), QUEST_ALREADY_LOCKED);
    V(printf("Properly handled already-locked state\n"));

    teardown();
}

TEST(test_door_edge_cases) {
    setup();

    // Test NULL door handling
    EXPECT_EQ(quest_door_lock(NULL), QUEST_INVALID_ARGS);
    EXPECT_EQ(quest_door_unlock(NULL, test_key), QUEST_INVALID_ARGS);
    V(printf("Handled NULL door cases\n"));
    
    // Test keyless door
    quest_door_t* keyless = quest_door_create(test_arena, NULL, NULL, 
                                            QUEST_DOOR_WOODEN, NULL, true, 
                                            quest_rtti_null());
    ASSERT(keyless != NULL, "Door creation failed");
    EXPECT_EQ(quest_door_unlock(keyless, test_key), QUEST_SUCCESS);
    EXPECT(!keyless->is_locked);
    V(printf("Verified keyless door behavior\n"));

    teardown();
}

#endif 

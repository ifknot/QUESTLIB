#ifndef TEST_QUEST_DOOR_H
#define TEST_QUEST_DOOR_H

#include "../TDD/tdd_macros.h"
#include "../MEM/mem_arena.h"

#include "quest_constants.h"
#include "quest_errors.h"
#include "quest_types.h"
#include "quest_door.h"
#include "quest_location.h"



#define QUEST_DOOR_TESTS &test_door_creation,     \
                         &test_door_unlocking,    \
                         &test_door_locking



static mem_arena_t arena;
static quest_rtti_t test_key = { .parts = {.type = QUEST_ITEM_KEY, .id = 42} };
static quest_rtti_t wrong_key = { .parts = {.type = QUEST_ITEM_KEY, .id = 99} };

TEST(test_door_creation) {
    quest_location_t* loc1 = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'A');
    quest_location_t* loc2 = quest_location_create(&arena, NULL, QUEST_LOCATION, NULL, 'B');
    quest_door_t* door = quest_door_create(&arena, loc1, loc2, QUEST_DOOR_WOODEN, NULL, true, test_key);
    
    EXPECT(door != NULL);
    EXPECT_EQ(door->base.rtti.parts.type, QUEST_DOOR_WOODEN);
    EXPECT(door->is_locked);
    EXPECT(quest_rtti_equals(&door->key, &test_key));
}

TEST(test_door_unlocking) {
    quest_door_t* door = quest_door_create(&arena, NULL, NULL, QUEST_DOOR_WOODEN, NULL, true, test_key);
    
    EXPECT_EQ(quest_door_unlock(door, &wrong_key), QUEST_WRONG_KEY);
    EXPECT(door->is_locked);
    
    EXPECT_EQ(quest_door_unlock(door, &test_key), QUEST_SUCCESS);
    EXPECT(!door->is_locked);
    
    EXPECT_EQ(quest_door_unlock(door, &test_key), QUEST_ALREADY_UNLOCKED);
}

TEST(test_door_locking) {
    quest_door_t* door = quest_door_create(&arena, NULL, NULL, QUEST_DOOR_WOODEN, NULL, false, test_key);
    
    EXPECT_EQ(quest_door_lock(door), QUEST_SUCCESS);
    EXPECT(door->is_locked);
    
    EXPECT_EQ(quest_door_lock(door), QUEST_ALREADY_LOCKED);
}

#endif

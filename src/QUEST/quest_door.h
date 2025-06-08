/**
 * @file quest_door.h
 * @brief Door connector between locations
 *
 * Specialized connector with locking mechanism
 */
#ifndef QUEST_DOOR_H
#define QUEST_DOOR_H

#include <stdbool.h>
#include "quest_connector.h"
#include "quest_composite.h"
#include "quest_errors.h"

typedef struct quest_door_t {
    quest_connector_t base;    ///< Base connector properties
    bool is_locked;           ///< Lock state
    quest_rtti_t key;         ///< Key required to unlock
} quest_door_t;

/**
 * @brief Initializes a door
 * @param door Door to initialize
 * @param loc1 First connected location
 * @param loc2 Second connected location
 * @param type Door type
 * @param info Descriptive information
 * @param locked Initial lock state
 * @param key Required key RTTI
 * @return Error code
 */
quest_error_t quest_door_init(
    quest_door_t* door,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
);

/**
 * @brief Creates a new door
 * @param arena Memory arena for allocation
 * @param loc1 First connected location
 * @param loc2 Second connected location
 * @param type Door type
 * @param info Descriptive information
 * @param locked Initial lock state
 * @param key Required key RTTI
 * @return New door or NULL on failure
 */
quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
);

/**
 * @brief Locks a door
 * @param door Door to lock
 * @return QUEST_SUCCESS if locked, QUEST_ALREADY_LOCKED if already locked
 */
quest_error_t quest_door_lock(quest_door_t* door);

/**
 * @brief Unlocks a door with a key
 * @param door Door to unlock
 * @param key Key being used
 * @return QUEST_SUCCESS if unlocked,
 *         QUEST_WRONG_KEY if key doesn't match,
 *         QUEST_ALREADY_UNLOCKED if door wasn't locked
 */
quest_error_t quest_door_unlock(quest_door_t* door, const quest_rtti_t* key);

#endif

/*
// Create adjacent locations
quest_location_t* kitchen = quest_location_create(arena, NULL, QUEST_LOCATION, kitchen_info, 'K');
quest_location_t* pantry = quest_location_create(arena, NULL, QUEST_LOCATION, pantry_info, 'P');

// Create a locked door between them
quest_door_t* door = quest_door_create(arena, kitchen, pantry, QUEST_DOOR_WOODEN,
                                      door_info, true, key_rtti);

// Connect east of kitchen to west of pantry
quest_connector_join(kitchen, pantry, CONN_E, (quest_connector_t*)door);

// Attempt to unlock with wrong key
quest_rtti_t wrong_key = {...};
if (quest_door_unlock(door, &wrong_key) == QUEST_WRONG_KEY) {
    printf("The key doesn't fit!\n");
}

// Unlock with correct key
quest_door_unlock(door, &correct_key);

// Verify unlocked state
assert(door->is_locked == false);

 */

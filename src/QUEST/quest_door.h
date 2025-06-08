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

#endif

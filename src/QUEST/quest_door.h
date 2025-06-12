/**
 * @file quest_door.h
 * @brief Door connector between locations
 *
 * Specialized connector 
 *
 * @code
 *
 * @endcode
 */
#ifndef QUEST_DOOR_H
#define QUEST_DOOR_H

#include <stdbool.h>
#include "quest_connector.h"
#include "quest_composite.h"
#include "quest_errors.h"

typedef enum {
    DOOR_IS_OPEN     = 0x0001, // can go striaght through - eg an open arch 
    DOOR_IS_LOCKABLE = 0x0002, // so can add lock(s) - or not eg open arch/window
    DOOR_HAS_LOCK    = 0x0004, // has one or more locks 
    DOOR_IS_RUSTY    = 0x0008  // has rusty hinges that might need some oil  
} quest_door_features_t;

typedef enum {
    DOOR_PAPER     = 0,    
    DOOR_WOOD      = 10,
    DOOR_IRON      = 20,
    DOOR_BRONZE    = 30
} quest_door_strengths_t;    // how easy it is to smash a door or safe to hide behind depending on NPC

typedef struct quest_door_t {
    quest_connector_t base;    
    quest_bitmask_t active_features;
    quest_size_t door_weight; // strength needed to move the door maybe level up to open
    quest_size_t door_strength; // can you smash the door?
} quest_door_t;

void quest_door_init(    // defaults to an open door with no lock
    quest_door_t* door,
     quest_type_t type,
    quest_info_t* info,
    quest_location_t* loc1,
    quest_location_t* loc2
);

quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_location_t* loc1,
    quest_location_t* loc2,
    quest_type_t type,
    quest_info_t* info,
    bool locked,
    quest_rtti_t key
);

quest_error_t quest_door_open(quest_door_t* door);

quest_error_t quest_door_close(quest_door_t* door);

quest_error_t quest_door_lock(quest_door_t* door, quest_key_t* key);

quest_error_t quest_door_unlock(quest_door_t* door, quest_key_t* key);

#endif

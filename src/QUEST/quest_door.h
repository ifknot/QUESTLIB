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
#include "quest_key.h"
#include "quest_types.h"

typedef enum {
    DOOR_IS_OPEN     = 0x0001,     // can go striaght through - eg an open arch
    DOOR_IS_LOCKABLE = 0x0002,     // so can add lock(s) - or not eg open arch/window
} quest_door_features_t;

typedef enum {
    DOOR_PAPER     = 16,
    DOOR_WOOD      = 32,
    DOOR_IRON      = 64,
    DOOR_BRONZE    = 128
} quest_door_strengths_t;    // how easy it is to smash a door or safe to hide behind depending on NPC

typedef struct quest_door_t {
    quest_connector_t base;
    quest_size_t weight; // strength needed to move the door maybe level up to open
    quest_size_t strength; // can you smash the door?
    quest_bitmask_t features; // upto 16 features
} quest_door_t;

void quest_door_init(    // defaults to an *unjoined*, open door with no lock and no hinges e.g an archway
    quest_door_t* door,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t weight,
    quest_size_t strength
);

// note doors are *not* joined to their locations when created
quest_door_t* quest_door_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t weight,
    quest_size_t strength
);

bool quest_door_is_open(quest_door_t* door);

quest_error_t quest_door_open(quest_door_t* door);

quest_error_t quest_door_close(quest_door_t* door);

void quest_door_dump(const quest_composite_t* comp, FILE* stream);

#endif

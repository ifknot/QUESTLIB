/**
 * @file quest_key_lock.h
 * @brief Unified key and lock system
 */
#ifndef QUEST_KEY_LOCK_H
#define QUEST_KEY_LOCK_H

#include "quest_composite.h"
#include "quest_types.h"

#define QUEST_KEY_WEAR_FACTOR 5    // repeatedly trying a key in a lock will erode its durability - eg found a fragile key only can try once!

typedef enum {
    QUEST_LOCK_IS_LOCKED     = 0x0001,
    QUEST_LOCK_IS_PICKABLE   = 0x0002,
    QUEST_LOCK_IS_RUSTY      = 0x0004
} quest_lock_states_t;

typedef struct quest_key_t {
    quest_component_t base;
    quest_combination_t code;    // a key can only have its code set by registering with a lock
    quest_size_t durability;
} quest_key_t;

typedef struct quest_lock_t {
    quest_component_t base;
    quest_combination_t code;         // a 32 bit lock code is generated randomly at creation
    quest_size_t key_count;           // number of imprinted keys
    quest_size_t key_max;             // maximum number of keys that can be registered to this lock
} quest_lock_t;

void quest_key_init(
    quest_key_t* key,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t durability
);

quest_key_t* quest_key_create(        // defaults to open, unpickable, not rusty
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t durability
);

void quest_lock_init(
    quest_lock_t* lock,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t key_max
);

quest_lock_t* quest_lock_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info,
    quest_size_t key_max
);

bool quest_key_lock_imprint(quest_lock_t* lock, quest_key_t* key);

bool quest_lock_is_locked(quest_lock_t* lock);

bool quest_lock_try_unlock(quest_lock_t* lock, quest_key_t* key);

bool quest_lock_try_lock(quest_lock_t* lock, quest_key_t* key);

//bool quest_lock_try_pick(quest_lock_t* lock, quest_pick_t* pick);

//spell?

#endif

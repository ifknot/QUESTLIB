/**
 * @file quest_key_lock.h
 * @brief Unified key and lock system
 */
#ifndef QUEST_KEY_LOCK_H
#define QUEST_KEY_LOCK_H

#include "quest_composite.h"
#include "quest_types.h"

#define QUEST_LOCK_KEY_MAX    4    // option of multiple keys for richer game play (same code) and even option copy keys
#define QUEST_KEY_WEAR_FACTOR 5    // repeatedly trying a key in a lock will erode its durability - eg found a fragile key only can try once!

typedef struct quest_key_t {
    quest_component_t base;
    quest_combination_t key_code;
    quest_size_t durability;        
} quest_key_t;

typedef struct quest_lock_t {
    quest_component_t base;
    quest_key_t* valid_keys[QUEST_LOCK_KEY_MAX];
    quest_size_t valid_keys_count;
    quest_combination_t lock_code;
} quest_lock_t;

void quest_key_init(
    quest_key_t* key, 
    quest_type_t type, 
    quest_info_t* info,
    quest_combination_t key_code,
    quest_size_t durability
);

quest_key_t* quest_key_create(
    mem_arena_t* arena, 
    quest_type_t type,
    quest_info_t* info, 
    quest_combination_t key_code,
    quest_size_t durability
);

void quest_lock_init(
    quest_lock_t* lock,     
    quest_type_t type, 
    quest_info_t* info,
    quest_combination_t lock_code
);

quest_lock_t* quest_lock_create(
    mem_arena_t* arena, 
    quest_type_t type,
    quest_info_t* info, 
    quest_combination_t lock_code
);

bool quest_key_lock_register(quest_lock_t* lock, quest_key_t* key);

bool quest_lock_try_unlock(quest_lock_t* lock, quest_key_t* key);

bool quest_lock_try_lock(quest_lock_t* lock, quest_key_t* key);

#endif

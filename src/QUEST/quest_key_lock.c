#include "quest_key_lock.h"

void quest_key_init(
    quest_key_t* key, 
    quest_component_t* parent,
    quest_type_t type, 
    quest_info_t* info,
    quest_combination_t key_code,
    quest_size_t durability
) {
    assert(key_code && "ZERO key_code!");
        
    quest_component_init(&key->base, parent, type, info);
    key->key_code = key_code; 
    key->durability = (durability > 100) ? 100 : durability;    // capped at 100%
    key->owner = NULL;
}

quest_key_t* quest_key_create(
    mem_arena_t* arena,
    quest_component_t* parent,
    quest_type_t type,
    quest_info_t* info, 
    quest_combination_t key_code,
    quest_size_t durability
) {
    assert(arena && "NULL memory arena!");
    
    quest_key_t* key = mem_arena_calloc(arena, sizeof(quest_key_t));
    quest_key_init(key, type, info, key_code, durability);
    return key;
}

void quest_lock_init(
    quest_lock_t* lock, 
    quest_component_t* parent,
    quest_type_t type, 
    quest_info_t* info,
    quest_combination_t lock_code
) {
    assert(lock_code && "ZERO key_code!");
    
    quest_component_init(&lock->base, parent, type, info);
    lock->clock_code = lock_code;
}

quest_lock_t* quest_lock_create(
    mem_arena_t* arena, 
    quest_type_t type,
    quest_info_t* info, 
    quest_combination_t lock_code
) {
    assert(arena && "NULL memory arena!");
    
    quest_lock_t* lock = mem_arena_calloc(arena, sizeof(quest_lock_t));
    quest_lock_init(lock, parent, type, info,lock_code);
    return lock;
}

bool quest_key_lock_register(
    quest_lock_t* lock,
    quest_key_t* key
) {
    assert(lock && "NULL lock!");
    assert(key && "NULL key!");

    if(lock->lock_code != key->key_code || lock->valid_keys_count == QUEST_LOCK_KEY_MAX) {
        return false;
    )
    lock->valid_keys[valid_keys_count++] = key;
    return true;
}

bool quest_key_lock_try_open(
    quest_key_t* key,
    quest_lock_t* lock
) {
    if (!key->durability) {
        return false;
    }
    for(int i = 0; i < valid_keys_count; ++i) {
        
    }
    
    for (int i = 0; i < 4; i++) {
        if (lock->valid_keys[i] &&
            lock->valid_keys[i]->key_code == key->key_code) {
            if (key->base.rtti.parts.type & KEY_PHYSICAL) {
                key->durability = (key->durability > 5) ?
                    key->durability - 5 : 0;
            }
            return true;
        }
    }
    return false;
}

bool quest_key_lock_set_state(
    quest_lock_t* lock,
    bool locked
) {
    bool previous = lock->base.features & LOCK_IS_LOCKED;
    if (locked) {
        lock->base.features |= LOCK_IS_LOCKED;
    } else {
        lock->base.features &= ~LOCK_IS_LOCKED;
    }
    return previous;
}

// ==================
// Picking Implementation
// ==================

bool quest_lock_pick_attempt(
    quest_lock_t* lock,
    uint8_t skill,
    uint8_t tools
) {
    if (!(lock->base.features & LOCK_IS_LOCKED) ||
        (lock->base.features & LOCK_IS_SEALED) ||
        lock->pick_resistance == 0) {
        return false;
    }

    uint8_t effective_diff = lock->pick_difficulty;

    if (lock->base.features & LOCK_IS_RUSTY) {
        effective_diff = (effective_diff * 70) / 100;
        if (quest_prng_roll(&quest_global_ctx, 30)) {
            tools = (tools * 50) / 100;
        }
    }

    bool trap_triggered = (lock->base.features & LOCK_IS_TRAPPED) &&
                         quest_prng_roll(&quest_global_ctx, 40 - skill/3);

    bool success = (skill + tools) / 2 > effective_diff;

    if (!success) {
        lock->pick_resistance =
            (lock->pick_resistance > 5) ?
            lock->pick_resistance - 5 : 0;
    }

    return success && !trap_triggered;
}

quest_bitmask_t quest_lock_get_required_tools(
    const quest_lock_t* lock
) {
    quest_bitmask_t tools = 0;

    if (lock->base.rtti.parts.type & LOCK_PHYSICAL) {
        tools |= TOOL_PICKSET | TOOL_TENSION_WRENCH;
        if (lock->base.features & LOCK_IS_RUSTY) {
            tools |= TOOL_OIL;
        }
    }
    else if (lock->base.rtti.parts.type & LOCK_COMBINATION) {
        tools |= TOOL_STETHOSCOPE;
    }
    else if (lock->base.rtti.parts.type & LOCK_MAGICAL) {
        tools |= TOOL_MAGIC_FOCUS;
    }

    return tools;
}

#include "quest_key_lock.h"

#include <string.h>

#include "quest_prng.h"

quest_prng_ctx_t quest_global_ctx;

// ==================
// Key Implementation
// ==================

void quest_key_init(
    quest_key_t* key,
    quest_type_t type,
    quest_info_t* info,
    uint32_t code,
    uint8_t durability
) {
    quest_component_init(&key->base, NULL, type, info);
    key->key_code = (code == 0) ? quest_prng_generate(&quest_global_ctx) : code;
    key->durability = (durability > 100) ? 100 : durability;
    key->owner = NULL;
}

quest_key_t* quest_key_create(
    mem_arena_t* arena,
    quest_type_t type,
    quest_info_t* info, uint32_t code,
    uint8_t durability
) {
    quest_key_t* key = mem_arena_calloc(arena, sizeof(quest_key_t));
    quest_key_init(key, type, info, code, durability);
    return key;
}

// ==================
// Lock Implementation
// ==================

void quest_lock_init(
    quest_lock_t* lock,
    quest_type_t type,
    quest_info_t* info,
    uint8_t difficulty
) {
    quest_component_init(&lock->base, NULL, type, info);
    lock->pick_difficulty = (difficulty > 100) ? 100 : difficulty;
    lock->combination = 0;
    lock->pick_resistance = 100;
    memset(lock->valid_keys, 0, sizeof(lock->valid_keys));
}

quest_lock_t* quest_lock_create(
    mem_arena_t* arena,
    quest_type_t type,
    quest_info_t* info,
    uint8_t difficulty
) {
    quest_lock_t* lock = mem_arena_calloc(arena, sizeof(quest_lock_t));
    quest_lock_init(lock, type, info, difficulty);
    return lock;
}

// ==================
// Key-Lock Interactions
// ==================

bool quest_key_lock_register(
    quest_lock_t* lock,
    quest_key_t* key
) {
    for (int i = 0; i < 4; i++) {
        if (!lock->valid_keys[i]) {
            lock->valid_keys[i] = key;
            return true;
        }
    }
    return false;
}

bool quest_key_lock_try_open(
    quest_key_t* key,
    quest_lock_t* lock
) {
    if (key->durability == 0) return false;

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

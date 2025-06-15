/**
 * @file quest_key_lock.h
 * @brief Unified key and lock system with picking mechanics
 *
 * @version 2.2
 * @changelog
 * - v2.2: Added comprehensive picking system
 * - v2.1: Unified key/lock implementation
 * - v2.0: Initial release
 */
#ifndef QUEST_KEY_LOCK_H
#define QUEST_KEY_LOCK_H

#include "quest_composite.h"

// ==================
// Key Types
// ==================
typedef enum {
    KEY_PHYSICAL    = 0x0001,
    KEY_ELECTRONIC  = 0x0002,
    KEY_CRYSTAL     = 0x0004,
    KEY_SKELETON    = 0x0008,
    KEY_BIOMETRIC   = 0x0010
} quest_key_type_t;

// ==================
// Lock Types
// ==================
typedef enum {
    LOCK_PHYSICAL    = 0x0100,
    LOCK_COMBINATION = 0x0200,
    LOCK_MAGICAL     = 0x0400,
    LOCK_BIOMETRIC   = 0x0800
} quest_lock_type_t;

// ==================
// Lock States
// ==================
typedef enum {
    LOCK_IS_LOCKED   = 0x0001,
    LOCK_IS_RUSTY    = 0x0002,
    LOCK_IS_TRAPPED  = 0x0004,
    LOCK_IS_SEALED   = 0x0008
} quest_lock_state_t;

// ==================
// Tool Types
// ==================
typedef enum {
    TOOL_PICKSET      = 0x01,
    TOOL_TENSION_WRENCH = 0x02,
    TOOL_STETHOSCOPE  = 0x04,
    TOOL_OIL          = 0x08,
    TOOL_MAGIC_FOCUS  = 0x10
} quest_tool_type_t;

// ==================
// Key Structure
// ==================
typedef struct quest_key_t {
    quest_component_t base;
    uint32_t key_code;
    uint8_t durability;
    struct quest_component_t* owner;
} quest_key_t;

// ==================
// Lock Structure
// ==================
typedef struct quest_lock_t {
    quest_component_t base;
    struct quest_key_t* valid_keys[4];
    uint8_t pick_difficulty;
    uint32_t combination;
    uint8_t pick_resistance;
} quest_lock_t;

// ==================
// Core API
// ==================

// Key Functions
void quest_key_init(quest_key_t* key, quest_type_t type, quest_info_t* info,
                   uint32_t code, uint8_t durability);
quest_key_t* quest_key_create(mem_arena_t* arena, quest_type_t type,
                            quest_info_t* info, uint32_t code, uint8_t durability);

// Lock Functions
void quest_lock_init(quest_lock_t* lock, quest_type_t type, quest_info_t* info,
                    uint8_t difficulty);
quest_lock_t* quest_lock_create(mem_arena_t* arena, quest_type_t type,
                              quest_info_t* info, uint8_t difficulty);

// ==================
// Interaction API
// ==================
bool quest_key_lock_register(quest_lock_t* lock, quest_key_t* key);
bool quest_key_lock_try_open(quest_key_t* key, quest_lock_t* lock);
bool quest_key_lock_set_state(quest_lock_t* lock, bool locked);
bool quest_lock_pick_attempt(quest_lock_t* lock, uint8_t skill, uint8_t tools);
quest_bitmask_t quest_lock_get_required_tools(const quest_lock_t* lock);

#endif

/**
 * @file quest_rtti.h
 * @brief Runtime Type Information (RTTI) System
 *
 * Provides type fingerprints for game objects, combining a 16-bit type ID
 * and a 16-bit unique instance ID into a 32-bit fingerprint.
 */
#ifndef QUEST_RTTI_H
#define QUEST_RTTI_H

#include "quest_types.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 32-bit RTTI fingerprint (type + unique instance ID).
 *
 * Layout:
 * - Bits 0-15: Unique instance ID (auto-incremented).
 * - Bits 16-31: Object type ID (user-defined).
 */
typedef uint32_t quest_fingerprint_t;

/** @brief 16-bit object type identifier (e.g., "Player", "Enemy"). */
typedef uint16_t quest_type_t;

/**
 * @brief RTTI container union
 *
 * Allows access to fingerprint as either:
 * - Combined 32-bit value
 * - Separate 16-bit components (type + UID)
 */
typedef union quest_rtti_t {
    uint32_t fingerprint;  /**< Combined 32-bit value */
    struct {
        uint16_t uid;     /**< Unique instance ID (LSB) */
        uint16_t type;    /**< Object type (MSB) */
    } parts;
} quest_rtti_t;

/**
 * @brief Creates a new RTTI fingerprint
 * @param type The object type identifier
 * @return New RTTI instance with auto-generated UID
 * @note Thread-safe if quest_next_uid() is thread-safe
 */
quest_rtti_t quest_rtti_create(quest_type_t type);

/**
 * @brief Gets the full 32-bit fingerprint
 * @param rtti RTTI instance to inspect
 * @return Combined type+UID fingerprint
 */
quest_fingerprint_t quest_get_fingerprint(quest_rtti_t rtti);

/**
 * @brief Gets the object type component
 * @param rtti RTTI instance to inspect
 * @return 16-bit type identifier
 */
quest_type_t quest_get_type(quest_rtti_t rtti);

/**
 * @brief Gets the unique instance ID
 * @param rtti RTTI instance to inspect
 * @return 16-bit unique identifier
 * @warning UIDs may wrap around after 65,535 creations
 */
quest_size_t quest_get_uid(quest_rtti_t rtti);

/**
 * @brief Type checking predicate
 * @param rtti RTTI instance to test
 * @param type Type to compare against
 * @return true if types match, false otherwise
 */
bool quest_is_typeof(quest_rtti_t rtti, quest_type_t type);

#endif

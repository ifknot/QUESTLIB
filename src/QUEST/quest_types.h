/**
 * @file quest_types.h
 * @brief Location data structures for Quest game engine
 *
 * Defines core types for storing location descriptions and metadata.
 */
#ifndef QUEST_TYPES_H
#define QUEST_TYPES_H

#include <stdint.h>

/**
 * @brief Type for size/length values throughout the Quest engine
 *
 * Uses 16-bit unsigned integers for memory efficiency in large world maps.
 * Maximum value: 65,535 (UINT16_MAX)
 */
typedef uint16_t quest_size_t;

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

typedef uint16_t quest_bitmask_t;

typedef enum {
    CONN_N = 0,
    CONN_NE,
    CONN_E,
    CONN_SE,
    CONN_S,
    CONN_SW,
    CONN_W,
    CONN_NW,
    CONN_UP,
    CONN_DOWN,
    CONN_STAIR
} quest_connection_direction_t;

// Connection direction bitflags
typedef enum {
    FLAG_N      = 0x0001,
    FLAG_NE     = 0x0002,
    FLAG_E      = 0x0004,
    FLAG_SE     = 0x0008,
    FLAG_S      = 0x0010,
    FLAG_SW     = 0x0020,
    FLAG_W      = 0x0040,
    FLAG_NW     = 0x0080,
    FLAG_UP     = 0x0100,
    FLAG_DOWN   = 0x0200,
    FLAG_STAIR  = 0x0400
} quest_connection_flags_t;

// Sterngth of quest objects
typedef enum {
    STRENGTH_NONE   = 0,
    STRENGTH_WEAK   = 8,
    STRENGTH_NORMAL = 16,
    STRENGTH_STRONG = 32
} quest_strength_t;

#endif

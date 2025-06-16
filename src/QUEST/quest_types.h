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

typedef quest_bitmask_t quest_feature_t;

typedef uint32_t quest_combination_t;

// Sterngth of quest objects
typedef enum {
    STRENGTH_NONE   = 0,
    STRENGTH_WEAK   = 8,
    STRENGTH_NORMAL = 16,
    STRENGTH_STRONG = 32
} quest_strength_t;

#endif

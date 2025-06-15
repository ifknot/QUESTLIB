/**
 * @file quest_features.h
 * @brief Standard component feature flags with byte segregation
 *
 * @memory_layout
 * +-------------------------------+
 * | 0-7: Common (all components)  |
 * | 8-15: Type-specific           |
 * +-------------------------------+
 *
 * @note Maximum 8 common and 8 type-specific features
 */
#pragma once

#include <stdint.h>

typedef uint16_t quest_feature_t;  // Now 16-bit total

// =============================================
// Common Features (Bits 0-7, all component types)
// =============================================
#define COMP_FEATURE_VISIBLE       0x0001  ///< Renders in-world (bit 0)
#define COMP_FEATURE_INTERACTABLE  0x0002  ///< Can be clicked/used (bit 1)
#define COMP_FEATURE_PERSISTENT    0x0004  ///< Survives area transitions (bit 2)
#define COMP_FEATURE_MOVABLE       0x0008  ///< Can be repositioned (bit 3)
// ... up to bit 7 (0x0080)

// =============================================
// Type-Specific Features (Bits 8-15)
// =============================================

// Door-specific (shifted left by 8 bits)
#define DOOR_FEATURE_LOCKED       0x0100  ///< bit 8
#define DOOR_FEATURE_AUTO_CLOSE   0x0200  ///< bit 9
#define DOOR_FEATURE_ONE_WAY      0x0400  ///< bit 10

// Container-specific
#define CONT_FEATURE_WEIGHT_LIMIT 0x0100  ///< bit 8 (same bit, different type)
#define CONT_FEATURE_OWNER_ONLY   0x0200  ///< bit 9

// Character-specific
#define CHAR_FEATURE_INVENTORY    0x0100  ///< bit 8
#define CHAR_FEATURE_CAN_ATTACK   0x0200  ///< bit 9

// =============================================
// Feature Macros
// =============================================

/**
 * @brief Creates type-specific feature mask
 * @param type Component type (QUEST_DOOR, etc.)
 * @param bit Bit position (0-7) within type-specific range
 */
#define TYPE_FEATURE(type, bit) (0x0100 << ((type) % 8) << (bit))

// Example: DOOR_FEATURE_LOCKED becomes TYPE_FEATURE(QUEST_DOOR, 0)

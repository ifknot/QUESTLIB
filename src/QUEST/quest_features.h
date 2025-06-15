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

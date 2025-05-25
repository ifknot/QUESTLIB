/**
 * @file str_types.h
 * @brief String utility type definitions
 *
 * @defgroup string_types String Utility Types
 * @{
 */

#ifndef STR_TYPES_H
#define STR_TYPES_H

#include <stdint.h>

/**
 * @brief Type for string sizes and lengths
 * @details Unsigned 32-bit integer type used consistently across string utilities
 *          for all size and length measurements. Provides consistent capacity
 *          for strings up to 4GB in length.
 */
typedef uint32_t str_size_t;

/**
 * @brief Type for string iteration positions
 * @details Unsigned 32-bit integer type used for maintaining and advancing
 *          positions when iterating through strings. Matches str_size_t for
 *          compatibility but semantically represents positions rather than sizes.
 */
typedef uint32_t str_iterator_t;

#endif

/** @} */ // end of string_types group

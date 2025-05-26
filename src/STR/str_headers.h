/**
 * @file str_headers.h
 * @brief Master include for all string processing utilities
 * @defgroup string_utils String Utilities
 * @{
 *
 * @brief Comprehensive string manipulation library for:
 *        - Memory management
 *        - Text processing
 *        - File operations
 *        - Data cleaning
 *
 * @note All components use consistent str_size_t for sizes
 */

#ifndef STR_HEADERS_H
#define STR_HEADERS_H

/* ----------------- Core Types & Constants ----------------- */
#include "str_types.h"       ///< Size/iterator types (str_size_t, str_iterator_t)
#include "str_constants.h"   ///< Character sets and limits (STR_WHITESPACE, etc.)

/* ----------------- Memory Management ----------------- */
#include "str_memory.h"      ///< Arena-based string allocation (str_make_string, etc.)

/* ----------------- Processing Functions ----------------- */
#include "str_processing.h"  ///< Core operations (comparison, sorting, etc.)
#include "str_scrubbing.h"   ///< Cleaning functions (trimming, punctuation removal)

/* ----------------- File Operations ----------------- */
#include "str_file_processing.h" ///< File-based string utilities

#endif

/** @} */ // end of string_utils group

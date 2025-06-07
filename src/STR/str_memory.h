/**
 * @file str_memory.h
 * @brief Arena-based string allocation utilities
 *
 * @defgroup string_memory String Memory Management
 * @{
 */

#ifndef STR_MEMORY_H
#define STR_MEMORY_H

#include "../MEM/mem_arena.h"
#include "str_types.h"

/**
 * @brief Creates a new empty string in an arena
 * @param arena Memory arena to allocate from
 * @param string_size Maximum capacity of the string (including null-terminator)
 * @return Pointer to newly allocated string
 *
 * @details Allocates and initializes a null-terminated string buffer.
 *          The returned string is guaranteed to:
 *          - Be properly null-terminated
 *          - Have the requested capacity
 *          - Be allocated from the specified arena
 *
 * @pre arena != NULL
 * @pre string_size > 0
 *
 * @warning The entire capacity is allocated but not initialized (except null-terminator)
 */
char* str_make_string(
    mem_arena_t* arena,
    str_size_t string_size
);

/**
 * @brief Creates an arena-allocated copy of a string
 * @param arena Memory arena to allocate from
 * @param source Null-terminated string to copy
 * @return Pointer to newly allocated copy
 *
 * @details Makes an exact copy of the source string including:
 *          - All characters up to null-terminator
 *          - Proper null-termination
 *          - Arena-allocated storage
 *
 * @pre arena != NULL
 * @pre source != NULL
 *
 * @note Automatically calculates required size (strlen(source) + 1)
 * @see str_make_string()
 * @see strcpy()
 */
char* str_make_copy(
    mem_arena_t* arena,
    const char* string
);

/**
 * @brief Creates an array of empty strings in an arena
 * @param arena Memory arena to allocate from
 * @param array_size Number of strings in array
 * @param string_size Size of each string buffer
 * @return Pointer to newly allocated string array
 *
 * @details Allocates:
 *          - An array of string pointers
 *          - Each string is initialized empty (null-terminated)
 *          - All memory comes from the specified arena
 *
 * @pre arena != NULL
 * @pre array_size > 0
 * @pre string_size > 0
 *
 * @warning All strings share the same fixed capacity
 * @note Useful for creating word lists or string tables
 */
char** str_make_string_array(
    mem_arena_t* mem_arena,
    str_size_t array_size,
    str_size_t string_size
);

#endif

/** @} */ // end of string_memory group

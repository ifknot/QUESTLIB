/**
 * @file parse_dictionary.h
 * @brief Interface for lexeme-token dictionary operations
 * @defgroup parse_dictionary Lexeme-Token Dictionary
 * @{
 */

#ifndef PARSE_DICTIONARY_H
#define PARSE_DICTIONARY_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "../MEM/mem_arena.h"
#include "../DOS/dos_services_files.h"
#include "parse_types.h"

/**
 * @brief Opaque dictionary structure
 */
typedef struct private_parse_dictionary parse_dictionary_t;

/* ====================== */
/* Dictionary Creation    */
/* ====================== */

/**
 * @brief Creates a new dictionary with specified capacity
 * @param arena Memory arena to allocate from
 * @param capacity Maximum number of entries
 * @return Pointer to newly created dictionary
 * @warning Both arena and capacity must be non-zero
 */
parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t capacity);

/**
 * @brief Creates a dictionary from a file
 * @param arena Memory arena to allocate from
 * @param path_name Path to dictionary file
 * @param dictionary Optional existing dictionary to populate
 * @return Pointer to created dictionary
 * @note File format should be lexeme-token pairs, one per line
 */
parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena,
                                                    const char* path_name,
                                                    parse_dictionary_t* dictionary);

/* ====================== */
/* Dictionary State       */
/* ====================== */

/**
 * @brief Gets the current number of entries
 * @param dict Dictionary to query
 * @return Number of active entries
 */
size_t parse_dictionary_size(const parse_dictionary_t* dict);

/**
 * @brief Gets the maximum capacity
 * @param dict Dictionary to query
 * @return Maximum number of entries
 */
size_t parse_dictionary_capacity(const parse_dictionary_t* dict);

/**
 * @brief Checks if dictionary is empty
 * @param dict Dictionary to check
 * @return true if empty, false otherwise
 */
bool parse_dictionary_is_empty(const parse_dictionary_t* dict);

/**
 * @brief Checks if dictionary is full
 * @param dict Dictionary to check
 * @return true if full, false otherwise
 */
bool parse_dictionary_is_full(const parse_dictionary_t* dict);

/**
 * @brief Checks if dictionary is sorted
 * @param dict Dictionary to check
 * @return true if sorted, false otherwise
 */
bool parse_dictionary_is_sorted(const parse_dictionary_t* dict);

/* ====================== */
/* Dictionary Operations  */
/* ====================== */

/**
 * @brief Adds a new lexeme-token pair
 * @param dict Dictionary to modify
 * @param lexeme Lexeme string to add
 * @param token Associated token value
 * @return Index of new entry or error code (<0)
 */
int parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token);

/**
 * @brief Removes an entry by index
 * @param dict Dictionary to modify
 * @param index Index to remove
 * @return New size or error code (<0)
 */
int parse_dictionary_remove(parse_dictionary_t* dict, size_t index);

/**
 * @brief Clears all entries
 * @param dict Dictionary to clear
 */
void parse_dictionary_clear(parse_dictionary_t* dict);

/**
 * @brief Sorts the dictionary lexicographically
 * @param dict Dictionary to sort
 */
void parse_dictionary_sort(parse_dictionary_t* dict);

/* ====================== */
/* Dictionary Query       */
/* ====================== */

/**
 * @brief Searches for a lexeme
 * @param dict Dictionary to search
 * @param target Lexeme to find
 * @return Index of found lexeme or error code (<0)
 */
int parse_dictionary_search(const parse_dictionary_t* dict, const char* target);

/**
 * @brief Checks if dictionary contains lexeme
 * @param dict Dictionary to search
 * @param lexeme Lexeme to find
 * @return true if found, false otherwise
 */
bool parse_dictionary_contains(const parse_dictionary_t* dict, const char* lexeme);

/**
 * @brief Gets pair at specific index
 * @param dict Dictionary to query
 * @param index Index to retrieve
 * @return The lexeme-token pair
 */
parse_lexeme_token_pair_t parse_dictionary_at(const parse_dictionary_t* dict, size_t index);

/**
 * @brief Gets token for lexeme
 * @param dict Dictionary to query
 * @param lexeme Lexeme to find
 * @return Associated token value
 */
parse_token_t parse_dictionary_tokenize(const parse_dictionary_t* dict, char* lexeme);

/* ====================== */
/* Debug/Utility          */
/* ====================== */

/**
 * @brief Dumps dictionary contents to stream
 * @param output_stream Stream to write to
 * @param dict Dictionary to dump
 */
void parse_dictionary_dump(FILE* output_stream, const parse_dictionary_t* dict);

#endif

/** @} */ // End of parse_dictionary group

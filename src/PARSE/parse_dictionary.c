/**
 * @file parse_dictionary.c
 * @brief Implementation of a dictionary for lexeme-token pairs
 * @details Provides operations for creating, managing, and querying a dictionary
 *          that maps lexemes to tokens. Supports adding, removing, searching,
 *          and sorting operations.
 */

#include "parse_dictionary.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse_constants.h"
#include "parse_types.h"

/**
 * @brief Internal structure representing the dictionary
 * @private
 */
typedef struct private_parse_dictionary {
    size_t size;                        /**< Current number of entries */
    size_t capacity;                    /**< Maximum capacity of dictionary */
    bool is_sorted;                     /**< Flag indicating if dictionary is sorted */
    parse_lexeme_token_pair_t* pairs;   /**< Array of lexeme-token pairs */
} parse_dictionary_t;

/**
 * @brief Compare function for qsort to sort lexeme-token pairs
 * @private
 * @param a First pair to compare
 * @param b Second pair to compare
 * @return <0 if a < b, 0 if a == b, >0 if a > b (lexicographical order)
 */
static int private_compare_pairs(const void *a, const void *b) {
    const parse_lexeme_token_pair_t* pair_a = (const parse_lexeme_token_pair_t*)a;
    const parse_lexeme_token_pair_t* pair_b = (const parse_lexeme_token_pair_t*)b;
    return strcmp(pair_a->lexeme, pair_b->lexeme);
}

/**
 * @brief Reset a dictionary entry to empty state
 * @private
 * @param dict Dictionary to modify
 * @param index Index of entry to reset
 */
static void private_reset_pair(parse_dictionary_t* dict, size_t index) {
    dict->pairs[index].lexeme[0] = '\0';  // Null string
    dict->pairs[index].token = 0;         // Token 0
}

/**
 * @brief Creates a new dictionary with specified capacity
 * @param arena Memory arena to allocate from
 * @param capacity Maximum number of entries
 * @return Pointer to newly created dictionary
 * @warning Both arena and capacity must be non-zero
 */
parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t capacity) {
    assert(arena && capacity);
    parse_dictionary_t* dict = mem_arena_alloc(arena, sizeof(parse_dictionary_t));
    assert(dict);
    dict->size = 0;
    dict->capacity = capacity;
    dict->is_sorted = false;
    dict->pairs = mem_arena_alloc(arena, dict->capacity * sizeof(parse_lexeme_token_pair_t));
    for (size_t i = 0; i < dict->capacity; i++) {
        private_reset_pair(dict, i);
    }
    return dict;
}

/**
 * @brief Checks if the dictionary has reached its capacity
 * @param dict Dictionary to check
 * @return true if full (size == capacity), false otherwise
 * @warning dict must not be NULL
 */
bool parse_dictionary_is_full(const parse_dictionary_t* dict) {
    assert(dict != NULL);
    return dict->size >= dict->capacity;
}

/**
 * @brief Checks if the dictionary contains a specific lexeme
 * @param dict Dictionary to search
 * @param lexeme Lexeme to find
 * @return true if lexeme exists, false otherwise
 * @warning Requires dictionary to be sorted (will return false if unsorted)
 * @note Wrapper around parse_dictionary_search()
 */
bool parse_dictionary_contains(const parse_dictionary_t* dict, const char* lexeme) {
    assert(dict && lexeme);
    return parse_dictionary_search(dict, lexeme) >= 0;
}

/**
 * @brief Checks if the dictionary is empty
 * @param dict Dictionary to check
 * @return true if empty (size == 0), false otherwise
 * @warning dict must not be NULL
 */
bool parse_dictionary_is_empty(const parse_dictionary_t* dict) {
    assert(dict != NULL);
    return dict->size == 0;
}

/**
 * @brief Clears all entries from the dictionary
 * @param dict Dictionary to clear
 * @note Maintains current capacity but resets size to 0
 */
void parse_dictionary_clear(parse_dictionary_t* dict) {
    assert(dict != NULL);
    for (size_t i = 0; i < dict->size; i++) {
        private_reset_pair(dict, i);
    }
    dict->size = 0;
    dict->is_sorted = false;
}

/**
 * @brief Adds a new lexeme-token pair to the dictionary
 * @param dict Dictionary to add to
 * @param lexeme Lexeme string to add
 * @param token Associated token value
 * @return Index where pair was added or error code:
 *         - PARSE_DICTIONARY_FULL if dictionary is full
 *         - PARSE_DICTIONARY_DUPLICATE if lexeme already exists
 * @warning Dictionary must not be NULL and lexeme must not be NULL
 * @note Unsafe for concurrent access - caller must synchronize
 */
int parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token) {
    assert(dict && lexeme);
    if(parse_dictionary_is_full(dict)) {
        return PARSE_DICTIONARY_FULL;
    }
    if(parse_dictionary_contains(dict, lexeme)) {
        return PARSE_DICTIONARY_DUPLICATE;
    }
    strncpy(dict->pairs[dict->size].lexeme, lexeme, PARSE_MAX_LEXEME_LENGTH);
    dict->pairs[dict->size].lexeme[PARSE_MAX_LEXEME_LENGTH] = '\0';
    dict->pairs[dict->size].token = token;
    dict->is_sorted = false;
    return dict->size++;
}

/**
 * @brief Removes an entry from the dictionary by index
 * @param dict Dictionary to modify
 * @param index Index of entry to remove
 * @return New size of dictionary or error code:
 *         - PARSE_DICTIONARY_OUT_OF_BOUNDS if index is invalid
 * @warning Dictionary must not be NULL and index must be valid
 */
int parse_dictionary_remove(parse_dictionary_t* dict, size_t index) {
    assert(dict && index < dict->size);
    if(index >= dict->size                              // out of range
        || index < 0                                    // out of range or passed a search result error
        || dict->pairs[index].lexeme[0] == '\0'         // indexed entry is a null string
    ) {
        return PARSE_DICTIONARY_OUT_OF_BOUNDS;
    }
    if (index < dict->size - 1) {
        memmove(&dict->pairs[index], &dict->pairs[index + 1], (dict->size - index - 1) * sizeof(parse_lexeme_token_pair_t));
    }
    private_reset_pair(dict, dict->size - 1); // clear last entry
    dict->size--;
    dict->is_sorted = false;
    return dict->size;
}

/**
 * @brief Sorts the dictionary lexicographically by lexeme
 * @param dict Dictionary to sort
 * @warning Dictionary must not be NULL and must have size > 1
 * @note Sets is_sorted flag to true upon completion
 */
void parse_dictionary_sort(parse_dictionary_t* dict) {
    assert(dict && dict->size > 1);
    qsort(dict->pairs, dict->size, sizeof(parse_lexeme_token_pair_t), private_compare_pairs);
    dict->is_sorted = true;
}

/**
 * @brief Searches for a lexeme in the dictionary
 * @param dict Dictionary to search
 * @param target Lexeme to find
 * @return Index of found lexeme or error code:
 *         - PARSE_DICTIONARY_EMPTY if dictionary is empty
 *         - PARSE_DICTIONARY_NOT_SORTED if dictionary isn't sorted
 *         - PARSE_DICTIONARY_NOT_FOUND if lexeme not found
 * @warning Dictionary must be sorted before searching
 * @note Uses binary search for O(log n) performance
 */
int parse_dictionary_search(const parse_dictionary_t* dict, const char* target) {
    assert(dict && target);
    if(!dict->size) {
        return PARSE_DICTIONARY_EMPTY;
    }
    if(!dict->is_sorted) {
        return PARSE_DICTIONARY_NOT_SORTED;
    }
    size_t i = 0;   // first entry in dictionary
    size_t j = dict->size - 1; // last entry in dictionary
    while (i <= j) {
        int midpoint = i + (j - i) / 2; // calculate new midpoint
        int found = strcmp(dict->pairs[midpoint].lexeme, target);
        if (found == 0) {       // exact match is found
            return midpoint;
        } else if (found < 0) { // target is in the 'upper' half
            i = midpoint + 1;
        } else {                // target is in the 'lower' half
            j = midpoint - 1;
        }
    }
    return PARSE_DICTIONARY_NOT_FOUND;
}

/**
 * @brief Gets the token associated with a lexeme
 * @param dict Dictionary to query
 * @param lexeme Lexeme to find
 * @return Associated token value
 * @see parse_dictionary_search()
 */
parse_token_t parse_dictionary_tokenize(const parse_dictionary_t* dict, char* lexeme) {
    return parse_dictionary_at(dict, parse_dictionary_search(dict, lexeme)).token;
}

/**
 * @brief Gets the lexeme-token pair at a specific index
 * @param dict Dictionary to query
 * @param index Index of pair to retrieve
 * @return The lexeme-token pair at the specified index
 * @warning Index must be valid (0 <= index < size)
 */
parse_lexeme_token_pair_t parse_dictionary_at(const parse_dictionary_t* dict, size_t index) {
    assert(index < dict->size);
    return dict->pairs[index];
}

/**
 * @brief Gets the current number of entries in the dictionary
 * @param dict Dictionary to query
 * @return Number of entries
 */
size_t parse_dictionary_size(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->size;
}

/**
 * @brief Gets the maximum capacity of the dictionary
 * @param dict Dictionary to query
 * @return Maximum number of entries
 */
size_t parse_dictionary_capacity(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->capacity;
}

/**
 * @brief Checks if the dictionary is sorted
 * @param dict Dictionary to check
 * @return true if sorted, false otherwise
 */
bool parse_dictionary_is_sorted(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->is_sorted;
}

/**
 * @brief Dumps the entire dictionary contents to a stream
 * @param output_stream Stream to write to (e.g., stdout)
 * @param dict Dictionary to dump
 * @warning output_stream must be valid
 * @note Includes empty slots in output
 */
void parse_dictionary_dump(FILE* output_stream, const parse_dictionary_t* dict) {
    assert(output_stream);
    for(int i = 0; i < dict->capacity; i++) {
        fprintf(output_stream, "%s = %i\n", dict->pairs[i].lexeme, dict->pairs[i].token);
    }
    assert(fflush(output_stream) != EOF);
}

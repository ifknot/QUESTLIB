/**
 * @file parse_dictionary.c
 * @brief Implementation of lexeme-token dictionary operations
 * @ingroup parse_dictionary
 */

#include "parse_dictionary.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse_constants.h"
#include "parse_types.h"

/* ====================== */
/* Internal Structures    */
/* ====================== */

/**
 * @brief Internal dictionary representation
 * @private
 */
typedef struct private_parse_dictionary {
    parse_size_t size;                        /**< Current number of entries */
    parse_size_t capacity;                    /**< Maximum capacity */
    bool is_sorted;                     /**< Sort status flag */
    parse_lexeme_token_pair_t* pairs;   /**< Array of pairs */
} parse_dictionary_t;

/* ====================== */
/* Internal Helpers       */
/* ====================== */

static int private_compare_pairs(const void *a, const void *b) {
    const parse_lexeme_token_pair_t* pair_a = a;
    const parse_lexeme_token_pair_t* pair_b = b;
    return strcmp(pair_a->lexeme, pair_b->lexeme);
}

static void private_reset_pair(parse_dictionary_t* dict, size_t index) {
    dict->pairs[index].lexeme[0] = '\0';
    dict->pairs[index].token = 0;
}

/* ====================== */
/* Dictionary Creation    */
/* ====================== */

parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t capacity) {
    assert(arena && capacity);
    parse_dictionary_t* dict = mem_arena_alloc(arena, sizeof(parse_dictionary_t));
    assert(dict);

    dict->size = 0;
    dict->capacity = capacity;
    dict->is_sorted = false;
    dict->pairs = mem_arena_alloc(arena, capacity * sizeof(parse_lexeme_token_pair_t));

    for (size_t i = 0; i < capacity; i++) {
        private_reset_pair(dict, i);
    }
    return dict;
}

parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena,
                                                    const char* path_name,
                                                    parse_dictionary_t* dictionary) {
    // TODO: Implement file loading logic
    (void)arena; (void)path_name; (void)dictionary;
    return NULL;
}

/* ====================== */
/* Dictionary State       */
/* ====================== */

parse_size_t parse_dictionary_size(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->size;
}

parse_size_t parse_dictionary_capacity(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->capacity;
}

bool parse_dictionary_is_empty(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->size == 0;
}

bool parse_dictionary_is_full(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->size >= dict->capacity;
}

bool parse_dictionary_is_sorted(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->is_sorted;
}

/* ====================== */
/* Dictionary Operations  */
/* ====================== */

int parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token) {
    assert(dict && lexeme);

    if (parse_dictionary_is_full(dict)) {
        return PARSE_DICTIONARY_FULL;
    }
    if (dict->size > 0 && parse_dictionary_contains(dict, lexeme)) {
        return PARSE_DICTIONARY_DUPLICATE;
    }

    strncpy(dict->pairs[dict->size].lexeme, lexeme, PARSE_MAX_LEXEME_LENGTH);
    dict->pairs[dict->size].lexeme[PARSE_MAX_LEXEME_LENGTH] = '\0';
    dict->pairs[dict->size].token = token;
    dict->is_sorted = false;

    return dict->size++;
}

int parse_dictionary_remove(parse_dictionary_t* dict, size_t index) {
    assert(dict);

    if (index >= dict->size || dict->pairs[index].lexeme[0] == '\0') {
        return PARSE_DICTIONARY_OUT_OF_BOUNDS;
    }

    if (index < dict->size - 1) {
        memmove(&dict->pairs[index], &dict->pairs[index + 1],
               (dict->size - index - 1) * sizeof(parse_lexeme_token_pair_t));
    }

    private_reset_pair(dict, --dict->size);
    dict->is_sorted = false;

    return dict->size;
}

void parse_dictionary_clear(parse_dictionary_t* dict) {
    assert(dict);

    for (size_t i = 0; i < dict->size; i++) {
        private_reset_pair(dict, i);
    }
    dict->size = 0;
    dict->is_sorted = false;
}

void parse_dictionary_sort(parse_dictionary_t* dict) {
    assert(dict);

    if(dict->is_sorted) {
        return;
    }
    if (dict->size > 1) {
        qsort(dict->pairs, dict->size, sizeof(parse_lexeme_token_pair_t), private_compare_pairs);
    }
    dict->is_sorted = true;
}

/* ====================== */
/* Dictionary Query       */
/* ====================== */

/**
 * @brief Searches for a lexeme in the dictionary
 * @param dict Dictionary to search
 * @param target Lexeme to find
 * @return Index of found lexeme or error code (<0)
 */
int parse_dictionary_search(const parse_dictionary_t* dict, const char* target) {
    assert(dict && target);

    if (!dict->size) {
        return PARSE_DICTIONARY_EMPTY;
    }
    if (!dict->is_sorted) {
        return PARSE_DICTIONARY_NOT_SORTED;
    }

    int low = 0;
    int high = dict->size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(dict->pairs[mid].lexeme, target);

        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return PARSE_DICTIONARY_NOT_FOUND;
}

bool parse_dictionary_contains(const parse_dictionary_t* dict, const char* lexeme) {
    assert(dict && lexeme);
    return parse_dictionary_search(dict, lexeme) >= 0;
}

parse_lexeme_token_pair_t parse_dictionary_at(const parse_dictionary_t* dict, size_t index) {
    assert(dict && index < dict->size);
    return dict->pairs[index];
}

parse_token_t parse_dictionary_tokenize(const parse_dictionary_t* dict, char* lexeme) {
    assert(dict && lexeme);
    int index = parse_dictionary_search(dict, lexeme);
    return (index >= 0) ? dict->pairs[index].token : 0;
}

/* ====================== */
/* Debug/Utility          */
/* ====================== */

void parse_dictionary_dump(FILE* output_stream, const parse_dictionary_t* dict) {
    assert(output_stream && dict);

    for (size_t i = 0; i < dict->capacity; i++) {
        fprintf(output_stream, "%3zu: %-15s = %d\n",
               i, dict->pairs[i].lexeme, dict->pairs[i].token);
    }
    assert(fflush(output_stream) != EOF);
}

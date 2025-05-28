#include "parse_dictionary.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse_constants.h"
#include "parse_types.h"

// Opaque struct definition (only visible in this file)
typedef struct private_parse_dictionary {
    size_t size;
    size_t capacity;
    bool is_sorted;
    parse_lexeme_token_pair_t* pairs;
} parse_dictionary_t;

// private helper functions
static int private_compare_pairs(const void *a, const void *b) {
    const parse_lexeme_token_pair_t* pair_a = (const parse_lexeme_token_pair_t*)a;
    const parse_lexeme_token_pair_t* pair_b = (const parse_lexeme_token_pair_t*)b;
    return strcmp(pair_a->lexeme, pair_b->lexeme);
}

static void private_reset_pair(parse_dictionary_t* dict, size_t index) {
    dict->pairs[index].lexeme[0] = '\0';  // Null string
    dict->pairs[index].token = 0;         // Token 0
}

// public functions
parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t capacity) { // fixed size implementation
    assert(arena && capacity);
    parse_dictionary_t* dict = mem_arena_alloc(arena, sizeof(parse_dictionary_t));
    assert(dict);
    dict->size = 0;
    dict->capacity = capacity;
    dict->is_sorted = false;
    dict->pairs = mem_arena_alloc(arena, dict->capacity * sizeof(parse_lexeme_token_pair_t));
    for (size_t i = 0; i < dict->capacity; i++) { // initialize dictionary entries
        private_reset_pair(dict, i);
    }
    return dict;
}

//parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena, const char * path_name, parse_dictionary_t* dictionary) {
//
//}

int parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token) {
    assert(dict && lexeme);
    if(dict->size >= dict->capacity) {
        return PARSE_DICTIONARY_FULL;
    }
    if(parse_dictionary_search(dict, lexeme) >= 0) {
        return PARSE_DICTIONARY_DUPLICATE;
    }
    strncpy(dict->pairs[dict->size].lexeme, lexeme, PARSE_MAX_LEXEME_LENGTH);
    dict->pairs[dict->size].lexeme[PARSE_MAX_LEXEME_LENGTH] = '\0'; // strncpy if no null byte among the first n bytes of src, the string placed in dest will not be null-terminated.
    dict->pairs[dict->size].token = token;
    dict->is_sorted = false;
    return dict->size++;
}

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

void parse_dictionary_sort(parse_dictionary_t* dict) {
    assert(dict && dict->size > 1);
    qsort(dict->pairs, dict->size, sizeof(parse_lexeme_token_pair_t), private_compare_pairs);
    dict->is_sorted = true;
}

/*
* NB The C standard only guarantees that strcmp returns:
*    < 0 if the first string is "less than" the second.
*    0 if they are equal.
*    > 0 if the first string is "greater than" the second.
* The actual value (not just -1 or 1) is implementation-dependent.
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
        if (found) {
            return midpoint;
        } else if (found < 0) { // target is in the 'upper' half
            i = midpoint + 1;
        } else { // target is in the 'lower' half
            j = midpoint - 1;
        }
    }
    return PARSE_DICTIONARY_NOT_FOUND;
}

parse_token_t parse_dictionary_tokenize(const parse_dictionary_t* dict, char* lexeme) {
    return parse_dictionary_at(dict, parse_dictionary_search(dict, lexeme)).token;
}

parse_lexeme_token_pair_t parse_dictionary_at(const parse_dictionary_t* dict, size_t index) {
    assert(dict && index >= 0 && index < dict->size);
    return dict->pairs[index];
}

size_t parse_dictionary_size(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->size;
}

size_t parse_dictionary_capacity(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->capacity;
}

bool parse_dictionary_is_sorted(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->is_sorted;
}

void parse_dictionary_dump(FILE* output_stream, const parse_dictionary_t* dict) { // dumps entire dictionary including empty slots
    assert(output_stream);
    for(int i = 0; i < dict->capacity; i++) {
        fprintf(output_stream, "%s = %i\n", dict->pairs[i].lexeme, dict->pairs[i].token);
    }
    assert(fflush(output_stream) != EOF);
}

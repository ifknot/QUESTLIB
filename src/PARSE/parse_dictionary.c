#include "parse_dictionary.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "parse_constants.h"
#include "parse_types.h"

// Opaque struct definition (only visible in this file)
typedef struct private_parse_dictionary {
    size_t size;
    size_t capacity;
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
    dict->pairs = mem_arena_alloc(arena, dict->capacity * sizeof(parse_lexeme_token_pair_t));
    for (size_t i = 0; i < dict->capacity; i++) { // initialize dictionary entries
        private_reset_pair(dict, i);
    }
    return dict;
}

//parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena, const char * path_name, parse_dictionary_t* dictionary);

void parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token) {
    assert(dict && lexeme && dict->size < dict->capacity);
    // TODO use find to disallow duplicates
    strncpy(dict->pairs[dict->size].lexeme, lexeme, PARSE_MAX_LEXEME_LENGTH);
    dict->pairs[dict->size].lexeme[PARSE_MAX_LEXEME_LENGTH] = '\0'; // strncpy if no null byte among the first n bytes of src, the string placed in dest will not be null-terminated.
    dict->pairs[dict->size].token = token;
    dict->size++;
}

void parse_dictionary_remove(parse_dictionary_t* dict, size_t index) {
    assert(dict && index < dict->size);
    if(dict->pairs[index].lexeme[0] == '\0') {  // null string
        return;
    }
    if (index < dict->size - 1) {
        memmove(&dict->pairs[index], &dict->pairs[index + 1], (dict->size - index - 1) * sizeof(parse_lexeme_token_pair_t));
    }
    private_reset_pair(dict, dict->size - 1); // clear last entry
    dict->size--;
}

void parse_dictionary_sort(parse_dictionary_t* dict) {
    assert(dict && dict->size > 1);
    qsort(dict->pairs, dict->size, sizeof(parse_lexeme_token_pair_t), private_compare_pairs);
}

size_t parse_dictionary_find(parse_dictionary_t* dict, char* lexeme) {
    return 0;
}

size_t parse_dictionary_size(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->size;
}

size_t parse_dictionary_capacity(const parse_dictionary_t* dict) {
    assert(dict);
    return dict->capacity;
}


void parse_dictionary_dump(FILE* output_stream, parse_dictionary_t* dict) { // dumps entire dictionary including empty slots
    assert(output_stream);
    for(int i = 0; i < dict->capacity; i++) {
        fprintf(output_stream, "->%s<- = %i\n", dict->pairs[i].lexeme, dict->pairs[i].token);
    }
    assert(fflush(output_stream) != EOF);
}

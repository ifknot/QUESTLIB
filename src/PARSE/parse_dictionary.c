#include "parse_dictionary.h"

#include <assert.h>
#include <stdio.h>

#include "../STR/str_memory.h"

#include "parse_constants.h"
#include "parse_types.h"

// Opaque struct definition (only visible in this file)
typedef struct private_parse_dictionary {
    size_t size;
    size_t capacity;
    parse_lexeme_token_pair_t* pairs;
} parse_dictionary_t;

parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t capacity) {
    assert(arena && capacity);
    parse_dictionary_t* dict = mem_arena_alloc(arena, sizeof(parse_dictionary_t));
    assert(dict);
    dict->size = 0;
    dict->capacity = capacity;
    dict->pairs = mem_arena_alloc(arena, dict->capacity * sizeof(parse_lexeme_token_pair_t));
    for (size_t i = 0; i < dict->capacity; i++) {
        dict->pairs[i].lexeme[0] = '\0';  // Null string
        dict->pairs[i].token = 0;         // Token 0
    }
    return dict;
}

//parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena, const char * path_name, parse_dictionary_t* dictionary);

size_t parse_dictionary_size(const parse_dictionary_t* dictionary) {
    assert(dictionary);
    return dictionary->size;
}

void parse_dictionary_dump(FILE* output_stream, parse_dictionary_t* dictionary) {
    assert(output_stream);
    for(int i = 0; i < dictionary->capacity; i++) {
        fprintf(output_stream, "->%s<- = %i\n", dictionary->pairs[i]->lexeme, dictionary->pairs[i]->token);
    }
    assert(fflush(output_stream) != EOF);
}

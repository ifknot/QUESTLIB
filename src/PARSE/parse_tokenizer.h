#ifndef PARSE_TOKENIZER_H
#define PARSE_TOKENIZER_H

#include "../MEM/mem_arena.h"

#include "parse_dictionary.h"
#include "parse_types.h"

typedef struct {
    parse_token_t* tokens;
    parse_size_t size;
} parse_token_array_t;

parse_token_array_t parse_tokenize(mem_arena_t* arena, parse_dictionary_t* dict, char* input);

parse_token_array_t parse_tokenize_ignore(mem_arena_t* arena, parse_dictionary_t* dict, char* input, char** ignore_list);

#endif

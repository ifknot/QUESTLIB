#ifndef PARSE_DICTIONARY_H
#define PARSE_DICTIONARY_H

#include <stddef.h>
#include <stddef.h>
#include <stdbool.h>

#include "../MEM/mem_arena.h"
#include "../DOS/dos_services_files.h"

#include "parse_types.h"

typedef struct private_parse_dictionary parse_dictionary_t;

parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t size);

parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena, const char * path_name, parse_dictionary_t* dictionary);

int parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token);

int parse_dictionary_remove(parse_dictionary_t* dict, size_t index);

void parse_dictionary_sort(parse_dictionary_t* dict);

/*
Binary search for token index *requires* sorted dictionary and assert fails if not
*/
int parse_dictionary_search(const parse_dictionary_t* dict, const char* target);

parse_token_t parse_dictionary_tokenize(const parse_dictionary_t* dict, char* lexeme);

parse_lexeme_token_pair_t parse_dictionary_at(const parse_dictionary_t* dict, size_t index);

size_t parse_dictionary_size(const parse_dictionary_t* dict);

size_t parse_dictionary_capacity(const parse_dictionary_t* dict);

bool parse_dictionary_is_sorted(const parse_dictionary_t* dict);

void parse_dictionary_dump(FILE* output_stream, const parse_dictionary_t* dict);

#endif

#ifndef PARSE_DICTIONARY_H
#define PARSE_DICTIONARY_H

#include <stddef.h>
#include <stddef.h>

#include "../MEM/mem_arena.h"
#include "../DOS/dos_services_files.h"

#include "parse_types.h"

typedef struct private_parse_dictionary parse_dictionary_t;

parse_dictionary_t* parse_dictionary_create(mem_arena_t* arena, size_t size);

//parse_dictionary_t* parse_dictionary_create_from_file(mem_arena_t* arena, const char * path_name, parse_dictionary_t* dictionary);

void parse_dictionary_add(parse_dictionary_t* dict, char* lexeme, parse_token_t token);

void parse_dictionary_remove(parse_dictionary_t* dict, size_t index);

void parse_dictionary_sort(parse_dictionary_t* dict);

/*
Binary search for token index *requires* sorted dictionary 
*/
size_t parse_dictionary_find(const parse_dictionary_t* dict, const char* lexeme);

parse_token_t parse_dictionary_get(const parse_dictionary_t* dict, size_t index);

inline parse_token_t parse_dictionary_tokenize(parse_dictionary_t* dict, char* lexeme) {
    return parse_dictionary_at(dict, parse_dictionary_search(dict, lexeme)).token;
}

size_t parse_dictionary_size(const parse_dictionary_t* dict);

size_t parse_dictionary_capacity(const parse_dictionary_t* dict);

void parse_dictionary_dump(FILE* output_stream, const parse_dictionary_t* dict);

#endif


/*
* NB The C standard only guarantees that strcmp returns:
*    < 0 if the first string is "less than" the second.
*    0 if they are equal.
*    > 0 if the first string is "greater than" the second.
* The actual value (not just -1 or 1) is implementation-dependent.
*/
/*
str_token_t str_tokenize(const str_word_token_t* dictionary, str_size_t size, const char* target) { MOVE TO DIC
    assert(dictionary && target);
    str_size_t i = 0; // start of dictionary
    str_size_t j = size - 1; // end of dictionary
    while (i <= j) {
        int m = i + (j - i) / 2; // calculate new midpoint
        int found = strcmp(dictionary[m].word , target);
        if (found) {
            return dictionary[m].token;
        } else if (found < 0) { // target is in the 'upper' half
            i = m + 1;
        } else { // target is in the 'lower' half
            j = m - 1;
        }
    }
    return 0;
}*/

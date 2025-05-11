#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_types.h"

uint16_t str_count_words(const char* string);

uint16_t str_enumarate_words(const char* string, char** string_array);

//uint16_t str_tokenize(char* string, char** dictionary, str_token_t* tokens);

//uint16_t str_binary_search_dictionary(char* word, char** dictionary)

#endif

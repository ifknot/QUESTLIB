#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_types.h"

/**
* @brief count the number of distinct alpha-numeric locale specific "words" in a zero terminated string as separated by non-alpha-numeric character(s).
*/
uint16_t str_count_words(const char* string);

/**
* @brief copy distinct alpha-numeric locale specific "words" from a zero terminated string into an array of zero terminated strings.
* @note no bounds checking for the size of string_array!
*/
uint16_t str_enumarate_words(const char* string, char** string_array);

//uint16_t str_tokenize(char* string, char** dictionary, str_token_t* tokens);

//uint16_t str_binary_search_dictionary(char* word, char** dictionary)

#endif

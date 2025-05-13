#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_types.h"

uint16_t str_to_upper_case(char* string);

//uint16_t str_copy_to_upper_case(const char* source, char* destination);

uint16_t str_to_lower_case(char* string);

//uint16_t str_copy_to_lower_case(const char* source, char* destination);

/**
* @brief count the number of distinct alpha-numeric locale specific "words" in a zero terminated string as separated by non-alpha-numeric character(s).
*/
uint16_t str_count_words(const char* string);

/**
* @brief copy distinct alpha-numeric locale specific "words" from a zero terminated string into an array of zero terminated strings.
* @note no bounds checking for the size of string_array!
*/
uint16_t str_enumarate_words(const char* string, char** string_array);

/**
* @brief binary search dictionary for zero terminated string containing 1 alpha-numeric word and return its token value, or zero if not found.
*/
str_token_t str_tokenize(const char* word, const str_word_token_t* dictionary);

#endif

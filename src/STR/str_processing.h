#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_types.h"

char* str_to_upper_case(char* string);

//char* str_copy_to_upper_case(const char* source, char* destination);

char* str_to_lower_case(char* string);

//char* str_copy_to_lower_case(const char* source, char* destination);

//str_size_t str_count_lines(const char* string);

/**
* @brief count the number of distinct alpha-numeric locale specific words/compound words in a zero terminated string as separated by non-alpha-numeric character(s).
*/
str_size_t str_count_words(const char* string);

str_size_t str_read_word(const char* string, str_iterator_t* i, char* word, const str_size_t limit);

str_size_t str_read_line(const char* string, str_iterator_t* i, char* line, const str_size_t limit);

/**
* @brief copy distinct alpha-numeric locale specific "words" from a zero terminated string into an array of zero terminated strings.
* @note no bounds checking for the size of string_array!
*/
str_size_t str_enumarate_words(const char* string, char** string_array, const str_size_t string_limit, const str_size_t array_limit);

#endif

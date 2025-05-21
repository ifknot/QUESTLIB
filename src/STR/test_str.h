#ifndef TEST_STR_H
#define TEST_STR_H

#include "../TDD/tdd_macros.h"

str_size_t str_trim_character(char* string, const char target);

str_size_t str_trim_characters(char* string, const char* targets);

//str_size_t str_copy_trim_characters(const char* const source, destination, const char* targets_array);

str_size_t str_trim_whitespace(char* string)

//str_size_t str_copy_trim_whitespace(const char* source, char* destination);

//str_size_t str_remove_characters(char* source, const char* targets);

str_size_t str_remove_punctuation(char* string);

#endif

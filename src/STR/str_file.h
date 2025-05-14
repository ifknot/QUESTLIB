#ifndef STR_FILE_H
#define STR_FILE_H

#include <stdint.h>

#include "str_types.h"

uint32_t str_file_count_lines(char* filepath);

uint32_t str_file_count_words(char* filepath);

uint32_t str_file_read_words(char* filepath, char** string_array);

uint32_t str_file_read_dictionary(char* filepath, str_word_token_t* dictionary);

#endif

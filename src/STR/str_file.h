#ifndef STR_FILE_H
#define STR_FILE_H

#include <stdint.h>

#include "../DOS/dos_services_files.h"

#include "str_types.h"

str_size_t str_file_count_lines(dos_file_handle_t fhandle);

/**
* @note a word is only counted if starts with a letter, may contain numbers, may be hyphenated
*/
str_size_t str_file_count_words(dos_file_handle_t fhandle);

/**
* @note advances the file position pointer to the next character
*/
inline str_size_t str_file_read_char(dos_file_handle_t fhandle, char* chr) {
    return dos_read_file(fhandle, chr, 1);
}

/**
* @note a word is only counted if starts with a letter, may contain numbers, may be hyphenated
* @note advances the file position pointer to the next word
*/
str_size_t str_file_read_word(dos_file_handle_t fhandle, char* string);

/**
* @note advances the file position pointer to the start of the next line
*/
str_size_t str_file_read_line(dos_file_handle_t fhandle, char* string);

str_size_t str_load_words(const char * path_name, char** string_array);

str_size_t str_load_dictionary(const char * path_name, str_word_token_t* dictionary);

#endif

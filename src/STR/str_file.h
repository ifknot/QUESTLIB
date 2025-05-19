#ifndef STR_FILE_H
#define STR_FILE_H

#include <stdint.h>

#include "../DOS/dos_services_files.h"

#include "str_types.h"

str_size_t str_file_count_lines(const dos_file_handle_t fhandle);

/**
* @note a word is only counted if starts with a letter, may contain numbers, may be compound (hyphen and/or apostrophe)
*/
str_size_t str_file_count_words(const dos_file_handle_t fhandle);

/**
* @note advances the file position pointer to the next character
*/
inline str_size_t str_file_read_char(const dos_file_handle_t fhandle, char* chr) {
    return dos_read_file(fhandle, chr, 1);
}

/**
* @note a word is only counted if starts with a letter, may contain numbers, may be compound (hyphen and/or apostrophe)
* @note advances the file position pointer to the next word
*/
str_size_t str_file_read_word(const dos_file_handle_t fhandle, char* word, const str_size_t limit);

/**
* @note advances the file position pointer to the start of the next line
*/
str_size_t str_file_read_line(const dos_file_handle_t fhandle, char* line, const str_size_t limit);

str_size_t str_file_enumerate_words(const char * path_name, char** string_array, const str_size_t string_limit, const str_size_t array_limit);

//TODO put in DIC str_size_t str_load_dictionary(const char * path_name, str_word_token_t* dictionary, const str_size_t limit);

#endif

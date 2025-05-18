#ifndef STR_FILE_H
#define STR_FILE_H

#include <stdint.h>

#include "../DOS/dos_services_files.h"

#include "str_types.h"

str_size_t str_file_count_lines(char* filepath);

str_size_t str_file_count_words(char* filepath);

inline str_size_t str_file_read_char(dos_file_handle_t fhandle, char* chr) {
    return dos_read_file(fhandle, chr, 1);
}

str_size_t str_file_read_line(char* filepath, char* string);

str_size_t str_file_read_words(char* filepath, char** string_array);

str_size_t str_file_load_dictionary(char* filepath, str_word_token_t* dictionary);

#endif

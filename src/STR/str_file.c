#include "str_file.h"

#include <assert.h>
#include <ctype.h>
#include <assert.h>
//#include <stdio.h>

#include "../DOS/dos_services_files.h"

str_size_t str_file_count_lines(const dos_file_handle_t fhandle) {
    assert(fhandle);
    dos_file_position_t fpos = dos_move_file_pointer(fhandle, 0, FSEEK_CUR);   // copy current file poistion pointer
    str_size_t lines = 0;
    char chr;
    while(str_file_read_char(fhandle, &chr)) {
        if (chr == '\n') {
            lines++;
        }
    }
    dos_move_file_pointer(fhandle, fpos, FSEEK_SET);    // restore position
    return lines;
}

str_size_t str_file_count_words(const dos_file_handle_t fhandle) {
    assert(fhandle);
    dos_file_position_t fpos = dos_move_file_pointer(fhandle, 0, FSEEK_CUR);   // copy current file poistion pointer
    str_size_t words = 0;
    char chr;
    while(str_file_read_char(fhandle, &chr)) {
        if(isalpha(chr)) {  // only consider a word if starts with a letter
            words++;
            while(str_file_read_char(fhandle, &chr) && (isalnum(chr) || chr == '\''|| chr == '-')); // apostophe and hyphen signify a compound word
        }
    }
    dos_move_file_pointer(fhandle, fpos, FSEEK_SET);    // restore position
    return words;
}

str_size_t str_file_read_word(const dos_file_handle_t fhandle, char* word, const str_size_t limit) {
    assert(fhandle && word && limit);
    str_size_t i = 0; // destination index
    char chr;
    while(str_file_read_char(fhandle, &chr) && !isalpha(chr)); //ignore everything until start of a word found or eof
    if(isalpha(chr)) {  // only consider a word if starts with a letter
        word[i++] = chr;
        while(i < limit && str_file_read_char(fhandle, &chr) && (isalnum(chr) || chr == '\''|| chr == '-')) { // apostophe and hyphen signify a compound word
            word[i++] = chr;
        }
        word[i] = 0; // terminate string
    }
    return i;
}

str_size_t str_file_read_line(const dos_file_handle_t fhandle, char* line, const str_size_t limit) {
    assert(fhandle && line && limit);
    str_size_t i = 0;
    char chr;
    while(str_file_read_char(fhandle, &chr) && i < limit && chr != '\n') {
        line[i++] = chr;
    }
    line[i] = 0;
    return i;
}

str_size_t str_file_enumerate_words(const char * path_name, char** string_array, const str_size_t string_limit, const str_size_t array_limit) {
    assert(path_name && string_array && string_limit && array_limit);
    dos_file_handle_t f = dos_open_file(path_name, ACCESS_READ_ONLY);
    assert(f);
TODO
    dos_close_file(f);
}

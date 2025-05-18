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

str_size_t str_file_read_word(const dos_file_handle_t fhandle, char* string, const str_size_t limit) {
    assert(fhandle && string);
    str_size_t i = 0; // character index
    char chr;
    while(str_file_read_char(fhandle, &chr) && !isalpha(chr)); //ignore everything until start of a word found or eof
    if(isalpha(chr) && i < limit) {  // only consider a word if starts with a letter
        string[i++] = chr;
        while(i < limit && str_file_read_char(fhandle, &chr) && (isalnum(chr) || chr == '\''|| chr == '-')) { // apostophe and hyphen signify a compound word
            string[i++] = chr;
        }
        string[i] = 0; // terminate string
    }
    return i;
}

str_size_t str_file_read_line(const dos_file_handle_t fhandle, char* string, const str_size_t limit) {
    assert(fhandle && string);
    str_size_t i = 0;
    char chr;
    while(str_file_read_char(fhandle, &chr) && i < limit && chr != '\n') {
        string[i++] = chr;
    }
    string[i] = 0;
    return i;
}

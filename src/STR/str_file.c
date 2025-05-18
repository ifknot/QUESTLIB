#include "str_file.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "../DOS/dos_services_files.h"

uint32_t str_file_count_lines(char* filepath) {
    uint32_t lines = 0;
    char ch;
    dos_file_handle_t f = dos_open_file(filepath, ACCESS_READ_ONLY);
    if(!f) {
        return lines;
    }
    while(dos_read_file(f, &ch, 1)) {
        if (ch == '\n') {
            lines++;
        }
    }
    dos_close_file(f);
    return lines;
}

uint32_t str_file_count_words(char* filepath) {
    uint32_t words = 0;
    char chr;
    dos_file_handle_t f = dos_open_file(filepath, ACCESS_READ_ONLY);
    if(!f) {
        return words;
    }
    while(str_file_read_char(f, &chr)) {
        if(isalnum(chr)) {
            words++;
            while(str_file_read_char(f, &chr) && (isalnum(chr) || chr == '-')); // hyphen signifies a compound word
        }
    }
    dos_close_file(f);
    return words;
}

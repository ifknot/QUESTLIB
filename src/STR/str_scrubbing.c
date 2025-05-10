#include "str_scrubbing.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

uint16_t str_trim_character(char* zsource, char c) {
    uint16_t i = 0; // search index;
    uint16_t j = 0; // copy index;
    bool leading = (zsource[i] == c) ? true : false;
    // if leading trim character(s) skip them
    while (leading && zsource[i] == c && zsource[i++]) {}
    while (zsource[i]) {
        zsource[j++] = zsource[i++];
    }
    zsource[j] = 0; // terminate the copied string
    return i;
}

uint16_t str_trim_characters(char* zsource, char* zcharacters) {
    uint16_t i = 0;
    char c = zcharacters[i++];
    while (c) {
       printf("%X\n", c);
       c = zcharacters[i++];
    };
    return i;
}

uint16_t str_remove_characters(char* zsource, char* const zcharacters) {
    uint16_t trimmed = 0;

    return trimmed;
}

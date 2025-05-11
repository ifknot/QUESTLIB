#include "str_processing.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "str_constants.h"

uint16_t str_count_words(const char* zsource) {
    uint16_t i = 0; // char index
    uint16_t n = 0; // word count
    while(zsource[i]) {
        if(isalnum(zsource[i])) {
            n++;    // found a word
            while(isalnum(zsource[i])) { // skip rest of word
                i++;
            };
        } else {
            i++;
        }
    }
    return n;
}

uint16_t str_enumarate_words(const char* zsource, char** zenumerated) {
    uint16_t i = 0; // source index
    uint16_t n = 0; // word count
    char word[STR_MAX_WORD_LENGTH];
    while(zsource[i]) {
        if(isalnum(zsource[i])) {
            n++;    // found a word
            uint16_t j = 0;
            word[j++] = zsource[i++];
            while(isalnum(zsource[i])) {
                word[j++] = zsource[i++];
            }
            word[j] = 0;
            printf("%s\n", word);
        } else {
            i++;
        }
    }
    return n;
}

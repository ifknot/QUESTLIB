#include "str_processing.h"

#include <stdint.h>
//#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "str_constants.h"

uint16_t str_count_words(const char* zsource) {
    uint16_t i = 0; // char index
    uint16_t n = 0; // word count
    char word[STR_MAX_WORD_LENGTH];
    bool isword = false;
    while(zsource[i]) {
        if (isblank(zsource[i])) {
            isword= false;
        } else if (!isword) {
            isword = true;
            n++;
        }
        i++;
    }
    return n;
}

uint16_t str_enumarate_words(const char* zsource, char** zenumerated) {

}

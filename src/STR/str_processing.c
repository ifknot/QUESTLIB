#include "str_processing.h"

#include <stdint.h>
//#include <stdio.h>
#include <ctype.h>
#include <assert.h>

char* str_to_upper_case(char* string) {
   assert(string);
    char* p = string;
    for (; *p; *p = toupper(*p), p++);
    return string;
}

char* void str_to_lower_case(char* string) {
    assert(string);
    char* p = string;
    for (; *p; *p = tolower(*p), p++);
    return string;
}


uint16_t str_count_words(const char* string) {
    assert(string);
    uint16_t i = 0; // char index
    uint16_t n = 0; // word count
    while(string[i]) {
        if(isalnum(string[i])) {
            n++;    // found a word
            while(isalnum(string[i])) { // skip rest of word
                i++;
            };
        } else {
            i++;
        }
    }
    return n;
}

uint16_t str_enumarate_words(const char* string, char** string_array) {
   assert(string && string_array);
    uint16_t i = 0; // source index
    uint16_t n = 0; // word count
    while(string[i]) {
        if(isalnum(string[i])) { // found a word
            uint16_t j = 0;
            string_array[n][j++] = string[i++]; // copy first letter into list slot
            while(isalnum(string[i])) {
                string_array[n][j++] = string[i++]; // copy any other letters
            }
            string_array[n++][j] = 0; // terminate the copied string and count the word
        } else {
            i++;
        }
    }
    return n;
}

/*
* NB The C standard only guarantees that strcmp returns:
*    < 0 if the first string is "less than" the second.
*    0 if they are equal.
*    > 0 if the first string is "greater than" the second.
* The actual value (not just -1 or 1) is implementation-dependent.
*/
str_token_t str_tokenize(const str_word_token_t* dictionary, uint16_t size, const char* target);
    assert(dictionary && target);
    uint16_t i = 0; // start of dictionary
    uint16_t j = size - 1; // end of dictionary 
    while (i <= j) {
        int m = i + (j - i) / 2; // calculate new midpoint
        int found = strcmp(dictionary[m].word , target);
        if (found) {
            return dictionary[m].token
        } else if (found < 0) { // target is in the 'upper' half
            i = m + 1;
        } else { // target is in the 'lower' half
            j = m - 1;
        }
    }
    return 0;
}


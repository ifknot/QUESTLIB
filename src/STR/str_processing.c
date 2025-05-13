#include "str_processing.h"

#include <stdint.h>
//#include <stdio.h>
#include <ctype.h>

uint16_t str_to_upper_case(char* string) {
    uint16_t i = 0; // char index
    while(string[i]) {
        string[i]=toupper(string[i++]);
    }
}

uint16_t str_to_lower_case(char* string) {
    uint16_t i = 0; // char index
    while(string[i]) {
        string[i]=tolower(string[i++]);
    }
}

uint16_t str_count_words(const char* string) {
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

str_token_t str_tokenize(const char* word, const str_word_token_t* dictionary) {

}


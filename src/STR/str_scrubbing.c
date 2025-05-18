#include "str_scrubbing.h"

//#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>

str_size_t str_trim_character(char* string, const char target) {
    assert(string);
    str_size_t i = 0; // search index;
    str_size_t j = 0; // copy index;
    bool islead = (string[i++] == target) ? true : false; // leading target?
    while(islead && string[i] == target && string[i++]); // skip them
    while (string[i]) {
        if(string[i] == target) {
            string[j++] = string[i++]; // allow 1 copy
            while(string[i] == target) { // skip any others
                i++;
            };
        }
        else { // let the rest through
            string[j++] = string[i++];
        }
    }
    j = (string[j - 1] == target) ? j - 1 : j; // check for trailing target
    string[j] = 0; // terminate the copied string
    return i - j;
}

str_size_t str_trim_characters(char* string, const char* targets) {
     assert(string && targets);
    str_size_t i = 0; // source index
    while (targets[i]) { // convert all targets to target[0]
       str_size_t j = 0;
       while(string[j]) {
           if(string[j] == targets[i]) {
               string[j] = targets[0];
           }
           j++;
       }
       i++;
    };
    return str_trim_character(string, targets[0]);
}

str_size_t str_remove_punctuation(char* string) {
     assert(string);
    str_size_t i = 0; // search index;
    str_size_t j = 0; // copy index;
    while(string[i]) {
        if(ispunct(string[i])) {
            i++;
        }
        string[j++] = string[i++];
    }
    return i - j;
}

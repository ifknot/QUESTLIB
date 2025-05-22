#include "str_processing.h"
#include "str_types.h"

//#include <stdio.h>
#include <ctype.h>
#include <assert.h>
//#include <string.h>

char* str_to_upper_case(char* string) {
   assert(string);
    char* p = string;
    for (; *p; *p = toupper(*p), p++);
    return string;
}

char* str_to_lower_case(char* string) {
    assert(string);
    char* p = string;
    for (; *p; *p = tolower(*p), p++);
    return string;
}

str_size_t str_count_words(const char* string) {
    assert(string);
    str_size_t i = 0; // char index
    str_size_t n = 0; // word count
    while(string[i]) {
        if(isalnum(string[i])) {
            n++;    // found a word
            while(isalnum(string[i]) || string[i] == '\'' || string[i] == '-') { // skip words, compound words, and words with number e.g C3PO
                i++;
            };
        } else {
            i++;
        }
    }
    return n;
}

str_size_t str_read_word(const char* string, str_iterator_t* i, char* word, const str_size_t limit) {
   assert(string && word && limit);
   str_size_t j = 0; // desitination index
   char chr;
    while(string[*i] && !isalpha(string[*i])) { //ignore everything until start of a word found or zero terminator
       (*i)++;
    }
     word[j++] = string[(*i)++];
     while(j < limit && string[*i] && (isalnum(string[*i]) || chr == '\''|| chr == '-')) { // apostophe and hyphen signify a compound word
         word[j++] = string[(*i)++];
     }
     word[j] = 0; // terminate word string
    return j;
}

str_size_t str_read_line(const char* string, str_iterator_t* i, char* line, const str_size_t limit) {
   assert(string && line && limit);
   str_size_t j = 0; // desitination index
   char chr;
   while(j < limit && string[*i] && string[*i] != '\n') {
        line[j++] = string[(*i)++];
    }
    line[j] = 0;
    return j;
}

str_size_t str_enumarate_words(const char* string, char** string_array, const str_size_t string_limit, const str_size_t array_limit) {
   assert(string && string_array && string_limit && array_limit);
    str_size_t i = 0; // word index
    while(i < array_limit && string[*i]) {
        str_read_word(string, &i, string_array[i], string_limit);
    }
    return i;
}

/*
* NB The C standard only guarantees that strcmp returns:
*    < 0 if the first string is "less than" the second.
*    0 if they are equal.
*    > 0 if the first string is "greater than" the second.
* The actual value (not just -1 or 1) is implementation-dependent.
*/
/*
// MOVE TO DIC
str_token_t str_tokenize(const str_word_token_t* dictionary, str_size_t size, const char* target) { MOVE TO DIC
    assert(dictionary && target);
    str_size_t i = 0; // start of dictionary
    str_size_t j = size - 1; // end of dictionary
    while (i <= j) {
        int m = i + (j - i) / 2; // calculate new midpoint
        int found = strcmp(dictionary[m].word , target);
        if (found) {
            return dictionary[m].token;
        } else if (found < 0) { // target is in the 'upper' half
            i = m + 1;
        } else { // target is in the 'lower' half
            j = m - 1;
        }
    }
    return 0;
}*/

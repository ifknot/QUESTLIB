#include "str_processing.h"
#include "str_types.h"

//#include <stdio.h>
#include <ctype.h>
#include <assert.h>

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

str_size_t str_count_lines(const char* string) {
    assert(string);
    str_size_t i = 0; // char index
    str_size_t n = (string[i]) ? 1 : 0; // initialize line count to 1 if string is not empty
    while(string[i]) { // iterate through string
        if(string[i] == '\n') {
            n++;    // found a line
        }
        i++;
    }
    return n;
}

str_size_t str_count_words(const char* string) {
    assert(string);
    str_size_t i = 0; // char index
    str_size_t n = 0; // word count
    while(string[i]) { // iterate through string
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

str_size_t str_read_word(const char* string, str_iterator_t* i, char* word, const str_size_t word_size) {
    assert(string);
    assert(word);
    assert(word_size);
    str_size_t j = 0; // desitination index
    char chr;
    while(string[*i] && !isalpha(string[*i])) { //ignore everything until start of a word found or zero terminator
       (*i)++;
    }
    while(j < word_size && string[*i] && (isalnum(string[*i]) || chr == '\''|| chr == '-')) { // apostophe and hyphen signify a compound word
        word[j++] = string[(*i)++];
    }
    word[j] = 0; // terminate word string
    return j;
}

str_size_t str_read_line(const char* string, str_iterator_t* i, char* line, const str_size_t line_size) {
   assert(string && line && line_size);
   str_size_t j = 0; // desitination index
   char chr;
   while(j < line_size && string[*i] && string[*i] != '\n') { // iterate through string until newline or limit reached
        line[j++] = string[(*i)++]; // copy character to line and increment index
    }
    line[j] = 0; // terminate line string
    return j;
}

str_size_t str_enumarate_words(const char* string,char** string_array, const str_size_t array_size, const str_size_t word_size) {
    assert(string && string_array && array_size && word_size);
    str_size_t i = 0; // string iterator
    int j = 0; // array index
    while(j < array_size && str_read_word(string, &i, string_array[j++], word_size)) {}
    return j - 1;
}

#ifndef PARSE_DICTIONARY_H
#define PARSE_DICTIONARY_H

#include "../STR/str_types.h"

#include "parse_types.h"

/**
* @brief binary search dictionary for zero terminated string containing 1 alpha-numeric word and return its token value, or zero if not found.
*/
//parse_token_t parse_tokenize(const parse_word_token_t* dictionary, str_size_t size, const char* target);


#endif


/*
* NB The C standard only guarantees that strcmp returns:
*    < 0 if the first string is "less than" the second.
*    0 if they are equal.
*    > 0 if the first string is "greater than" the second.
* The actual value (not just -1 or 1) is implementation-dependent.
*/
/*
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

#ifndef STR_TYPES_H
#define STR_TYPES_H

#include <stdint.h>

#include "str_constants.h"

typedef uint16_t str_token_t;

typedef struct {
    char word[STR_MAX_DICTIONARY_WORD_LENGTH];
    str_token_t token;
} str_word_token_t;

#endif

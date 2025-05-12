#ifndef STR_TYPES_H
#define STR_TYPES_H

#include <stdint.h>

typedef uint16_t str_token_t;

typedef struct {
    char word[15];
    str_token_t token;
} str_word_token_t;

#endif

#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#include <stdint.h>

typedef uint16_t parse_token_t;

typedef struct {
    char* word;
    parse_token_t token;
} parse_word_token_pair_t;

#endif

#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#include <stdint.h>

#include "parse_constants.h"

typedef uint16_t parse_token_t;

typedef struct {
    char lexeme[PARSE_MAX_LEXEME_LENGTH];   // fixed size lexeme entrys
    parse_token_t token;
} parse_lexeme_token_pair_t;

#endif

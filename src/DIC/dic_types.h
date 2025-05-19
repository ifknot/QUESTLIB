#ifndef DIC_TYPES_H
#define DIC_TYPES_H

#include "DIC_CONSTANTS.H"

typedef uint16_t dic_token_t;

typedef struct {
    char word[DIC_MAX_WORD_LENGTH];
    dic_token_t token;
} dic_word_token_pair_t;

#endif

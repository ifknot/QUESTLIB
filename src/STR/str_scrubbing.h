#ifndef STR_SCRUBBING_H
#define STR_SCRUBBING_H

#include <stdint.h>

#include "str_constants.h"


uint16_t str_trim_character(char* zsource, char target);

uint16_t str_trim_characters(char* zsource, char* zcharacters);

//uint16_t str_copy_trim_characters(char* const zsource, char* zdestination, char* const zcharacters);

inline uint16_t str_trim_whitespace(char* zsource) {
    return str_trim_characters(zsource, STR_WHITESPACE);
}

//uint16_t str_copy_trim_whitespace(char* const zsource, char* zdestination);

//uint16_t str_remove_characters(char* zsource, char* const zcharacters);

uint16_t str_remove_punctuation(char* zsource);

//uint16_t str_copy_remove_characters(char* const zsource, char* zdestination, char* const zcharacters);

#endif

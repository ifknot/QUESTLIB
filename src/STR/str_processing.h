#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_types.h"

uint16_t str_count_words(const char* zsource);

uint16_t str_enumarate_words(const char* zsource, char** zenumerated);

//uint16_t str_tokenize(char* zsource, char** zdictionary, str_token_t* tokens);

#endif

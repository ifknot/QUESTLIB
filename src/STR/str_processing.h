#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_types.h"

uint16_t str_tokenize(char* zsource, char** zdictionary, token_t* tokens);

#endif

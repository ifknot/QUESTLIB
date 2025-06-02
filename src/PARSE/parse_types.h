/**
 * @file parse_types.h
 * @brief Type definitions for the parsing system
 * @defgroup parse_types Parsing Type Definitions
 * @{
 */
#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#include <stdint.h>
#include "parse_constants.h"

typedef uint16_t parse_size_t;

/**
 * @brief Token type definition
 * @details Represents a unique identifier for lexemes in the parsing system.
 * @note the token is split into a 12 bit identifier and a 4 bit role such that:
 * The high nybble is used to code the lexeme as verb, object, particle, preposition, compound (so far) and 
 * 12 bits for verbs action eg go, take, examine, use, talk to, and inventory for objects their type eg room, 
 * blue key, sword, etc
 */
typedef uint16_t parse_token_t;

/**
 * Composed of upto 8 nybbles from the parse tokens from the input sentence that encode the input pattern, eg VERB OBJECT, VERB PREPOSITION OBJECT, VERB etc - in an extensible way.
 * This input pattern can then be used to dispatch to the appropriate handler with the associated 12 bit information 
 */
typedef uint32_t command_pattern_t;

/**
 * @brief Lexeme-token pair structure
 * @details Stores a mapping between a lexeme string and its corresponding token.
 * @note Lexemes are fixed-length for memory efficiency (see PARSE_MAX_LEXEME_LENGTH)
 */
typedef struct {
    char lexeme[PARSE_MAX_LEXEME_LENGTH + 1]; /**< Fixed-size buffer for lexeme string (+1 for null terminator) */
    parse_token_t token;                      /**< Associated token value */
} parse_lexeme_token_pair_t;

#endif

/** @} */ // End of parse_types group

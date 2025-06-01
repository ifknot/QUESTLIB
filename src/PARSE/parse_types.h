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

/**
 * @brief Token type definition
 * @details Represents a unique identifier for lexemes in the parsing system.
 * @note Uses uint16_t to allow for 65,536 unique tokens while being memory efficient.
 */
typedef uint16_t parse_token_t;

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

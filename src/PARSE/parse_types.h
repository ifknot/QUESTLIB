/**
 * @file parse_types.h
 * @brief Command parsing type system combining grammar and semantics
 * @defgroup parse_types Parsing Type System
 * @{
 */
#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#include <stdint.h>
#include "parse_constants.h"

typedef uint16_t parse_size_t;

/**
 * @brief Unified lexical token encoding grammar and semantics
 * @details Combines both grammatical role (4 bits) and semantic meaning (12 bits) in one value:
 * 
 * ┌───────────────────┬────────────────────┐
 * │ Grammar (4 bits)  │ Semantics (12 bits)│
 * │ (what the word is)│ (what it means)    │
 * └───────────────────┴────────────────────┘
 * 
 * @note Example encodings:
 * - "take"    = (VERB, TAKE_ID)
 * - "key"     = (OBJECT, KEY_ID) 
 * - "with"    = (PREPOSITION, WITH_ID)
 * - "quickly" = (PARTICLE, QUICKLY_ID)
 */
typedef uint16_t parse_token_t;

/**
 * @brief Sentence structure fingerprint
 * @details Compact representation of a sentence's grammatical structure formed by:
 * 1. Extracting the grammar nybbles from each token
 * 2. Packing up to 8 into a 32-bit values
 * 
 * Used to route commands to the correct handler based on structure.
 * 
 * @code
 * // "put key in box" becomes:
 * // VERB(put) OBJECT(key) PREPOSITION(in) OBJECT(box)
 * // Packed as: 0x1242
 * @endcode
 */
typedef uint32_t command_pattern_t;

/* ----------------- Token Components ----------------- */

/**
 * @name Token Anatomy
 * @brief Masks for accessing token parts
 * @{
 */
#define TOKEN_TYPE_MASK  0xF000  /**< Isolate grammar nybble */
#define TOKEN_VALUE_MASK 0x0FFF  /**< Isolate semantic value */
/** @} */

/**
 * @brief Grammatical roles
 * @description Determines how words function in sentence structures
 */
typedef enum {
    TOKEN_VERB        = 0x1000, /**< Actions (go, take) */
    TOKEN_OBJECT      = 0x2000, /**< Things (key, door) */
    TOKEN_PARTICLE    = 0x3000, /**< Modifiers (quickly) */
    TOKEN_PREPOSITION = 0x4000, /**< Relations (with, in) */
    TOKEN_COMPOUND    = 0x5000  /**< Phrases (look at) */
} TokenType;

/**
 * @name Token Accessors  
 * @brief Safe component extraction
 * @{
 */
#define GET_TOKEN_TYPE(t)  ((t) & TOKEN_TYPE_MASK)  /**< Get grammatical role */
#define GET_TOKEN_VALUE(t) ((t) & TOKEN_VALUE_MASK) /**< Get semantic meaning */
/** @} */

/* ----------------- Lexical Storage ----------------- */

/**
 * @brief Word-to-token mapping
 * @details Associates surface words with their encoded meanings
 */
typedef struct {
    char lexeme[PARSE_MAX_LEXEME_LENGTH + 1]; /**< Text form (+null terminator) */
    parse_token_t token;                      /**< Encoded grammar+semantics */
} parse_lexeme_token_pair_t;

#endif
/** @} */ // End parse_types group

/**
 * @file parse_types.h
 * @brief Command parsing type system with dual-purpose token encoding
 * @defgroup parse_types Parsing Type System
 * @{
 */
#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#include <stdint.h>
#include "parse_constants.h"

/**
 * @brief Standardized size type for parse library operations
 * @details Ensures consistent memory management across:
 * - Dictionary capacities (max 65,535 entries)
 * - Lexeme lengths
 * - Buffer sizes
 */
typedef uint16_t parse_size_t;

/**
 * @brief Dual-purpose token encoding
 * @details The 4-bit grammar nybble serves two critical roles:
 * 
 * 1. During Lexing:
 * ┌───────────────┐
 * │ Classifies    │
 * │ word types    │
 * └───────────────┘
 *   (verb/noun/preposition/etc)
 * 
 * 2. During Parsing:  
 * ┌───────────────┐
 * │ Builds        │
 * │ sentence      │
 * │ patterns      │
 * └───────────────┘
 *   (command structures)
 * 
 * Memory layout:
 * ┌────────────┬────────────────────┐
 * │4-bit role  │ 12-bit semantics   │
 * │(grammar)   │ (specific meaning) │
 * └────────────┴────────────────────┘
 */
typedef uint16_t parse_token_t;

/**
 * @brief Command pattern fingerprint
 * @details Compact grammar structure formed by packing token nybbles:
 * 
 * Example:
 * "pick up sword and stab troll" → 
 * [VERB][PARTICLE][OBJECT][CONJUNCTION][VERB][OBJECT] →
 * 0x131621
 */
typedef uint32_t parse_pattern_t;

/* ----------------- Token Components ----------------- */
#define TOKEN_TYPE_MASK  0xF000  /**< Mask for grammar role */
#define TOKEN_VALUE_MASK 0x0FFF  /**< Mask for semantic ID */

/**
 * @brief Grammar role classifications
 * @note The 4-bit type serves dual purpose for both:
 * - Token categorization during lexing
 * - Pattern building during parsing
 */
typedef enum {
    TOKEN_VERB        = 0x1000, /**< Actions (take, go) */
    TOKEN_OBJECT      = 0x2000, /**< Entities (sword, door) */
    TOKEN_PARTICLE    = 0x3000, /**< Phrasal elements (up, down) */
    TOKEN_PREPOSITION = 0x4000, /**< Relators (with, in) */
    TOKEN_COMPOUND    = 0x5000, /**< Multi-word units (look at) */
    TOKEN_CONJUNCTION = 0x6000  /**< Sentence combiners (and, or) */
} parse_token_grammar_t;

/**
 * @brief Conjunction subtypes
 * @details Modify command sequencing behavior:
 */
typedef enum {
    CONJ_AND  = 0x6001,  /**< Try all commands (A and B) */
    CONJ_THEN = 0x6002,  /**< Sequential execution (A then B) */
    CONJ_OR   = 0x6003   /**< First successful command (A or B) */
} parse_conjunction_t;

/* ----------------- Lexical Storage ----------------- */
typedef struct {
    char lexeme[PARSE_MAX_LEXEME_LENGTH + 1]; /**< Surface form */
    parse_token_t token;                      /**< Encoded role+meaning */
} parse_lexeme_token_pair_t;

#endif
/** @} */

/**
 * @file parse_constants.h
 * @brief Constants for parsing and dictionary operations
 * @defgroup parse_constants Parsing Constants
 * @{
 */
#ifndef PARSE_CONSTANTS_H
#define PARSE_CONSTANTS_H

/**
 * @brief Maximum length for lexemes in characters
 * @details Longer lexemes will be truncated to this length.
 * @note Includes space for null terminator (actual max content is PARSE_MAX_LEXEME_LENGTH-1)
 */
#define PARSE_MAX_LEXEME_LENGTH 15

/**
 * @name Dictionary Status Codes
 * @brief Return values for dictionary operations
 * @{
 */
#define PARSE_DICTIONARY_NOT_FOUND      -1  /**< Lexeme not found in dictionary */
#define PARSE_DICTIONARY_DUPLICATE      -2  /**< Lexeme already exists in dictionary */
#define PARSE_DICTIONARY_EMPTY          -3  /**< Dictionary contains no entries */
#define PARSE_DICTIONARY_FULL           -4  /**< Dictionary has reached capacity */
#define PARSE_DICTIONARY_SORTED         -5  /**< Dictionary is sorted (unused in current implementation) */
#define PARSE_DICTIONARY_NOT_SORTED     -6  /**< Dictionary needs sorting for this operation */
#define PARSE_DICTIONARY_OUT_OF_BOUNDS  -7  /**< Index parameter is invalid */
/** @} */

#endif

/** @} */ // End of parse_constants group

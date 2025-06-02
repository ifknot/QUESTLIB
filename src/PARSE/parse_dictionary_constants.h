/**
 * @file parse_constants.h
 * @brief Constants for dictionary operations
 * @defgroup parse_dictionary_constants Parsing Dictionary Constants
 * @{
 */
#ifndef PARSE_DICTIONARY_CONSTANTS_H
#define PARSE_DICTIONARY_CONSTANTS_H


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

/** @} */ // End of parse_dictionary_constants group

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

#endif

/** @} */ // End of parse_constants group

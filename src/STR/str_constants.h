/**
 * @file str_constants.h
 * @brief String utility constants and definitions
 *
 * @defgroup string_constants String Utility Constants
 * @{
 */

#ifndef STR_CONSTANTS_H
#define STR_CONSTANTS_H

/**
 * @brief Maximum word length for string operations
 * @details Defines a practical limit for word processing (24 characters).
 *          Based on the longest common non-technical English word:
 *          "incomprehensibilities" (21 letters).
 * @note This is a soft limit for buffer sizing, not a hard restriction.
 */
#define STR_MAX_WORD_LENGTH 24

/**
 * @brief Whitespace character set
 * @details Contains all standard whitespace characters:
 *          - Space ( )
 *          - Newline (\n)
 *          - Tab (\t)
 *          - Vertical tab (\v)
 *          - Backspace (\b)
 *          - Carriage return (\r)
 *          - Form feed (\f)
 *          - Alert (\a)
 * @note Used by str_trim_whitespace() and related functions
 */
static char STR_WHITESPACE[] = " \n\t\v\b\r\f\a";

/**
 * @brief Basic punctuation character set
 * @details Contains common punctuation marks:
 *          - Comma (,)
 *          - Period (.)
 *          - Semicolon (;)
 *          - Colon (:)
 *          - Question mark (?)
 *          - Exclamation mark (!)
 * @note Used by punctuation-handling functions
 * @note Can be extended for locale-specific needs
 */
static char STR_PUNCTUATION[] = ",.;:?!";

#endif

/** @} */ // end of string_constants group

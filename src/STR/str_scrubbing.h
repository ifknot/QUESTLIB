/**
 * @file str_scrubbing.h
 * @brief String cleaning and normalization utilities
 *
 * @defgroup string_scrubbing String Scrubbing Functions
 * @{
 */

#ifndef STR_SCRUBBING_H
#define STR_SCRUBBING_H

#include <stdint.h>

#include "str_constants.h"
#include "str_types.h"

/**
 * @brief Removes all occurrences of a character from a string in-place.
 *
 * @param[in,out] string Null-terminated string to modify (must not be NULL)
 * @param[in] target Character to remove from the string
 * @return Number of characters removed (str_size_t)
 *
 * @details Modifies the original string by removing all instances of the target character.
 *          The operation is performed in O(n) time with O(1) additional space.
 *
 * @pre string != NULL (checked by assert)
 *
 * @warning Modifies the input string directly
 * @warning Not thread-safe during modification
 *
 * @note Preserves the order of remaining characters
 * @note Always maintains proper null-termination
 * @note Returns 0 if no characters were removed
 * @note Case-sensitive comparison (use tolower/toupper for case-insensitive removal)
 */
str_size_t str_remove_character(char* string, const char target);

/**
* @brief remove from the zero terminated string any leading and trailing target characters and any contiguous duplicated target characters between words.
* @note trimming is not the same as removing characters.
*/
str_size_t str_trim_character(char* string, const char target);

//str_size_t str_copy_trim_character(const char* source, char* destination, const char* targets_array);

/**
 * @brief Normalizes and trims specified characters from a string.
 *
 * @param[in,out] string The string to process (modified in-place)
 * @param[in] targets Null-terminated array of characters to trim (must not be NULL)
 * @return Number of characters removed (str_size_t)
 *
 * @details Performs three operations:
 *          1. Converts all characters in `targets` to the first character in `targets`
 *          2. Trims leading and trailing occurrences of the first `targets` character
 *          3. Collapses consecutive occurrences between words to single characters
 *
 * @pre string != NULL
 * @pre targets != NULL
 * @pre targets[0] != '\0' (at least one target character must be specified)
 *
 * @warning Modifies the string in-place
 * @warning Uses first character in `targets` as the normalization target
 * @warning Not thread-safe during modification
 *
 * @note More flexible than str_trim_whitespace() for arbitrary character sets
 * @note Preserves null-termination
 * @note Returns 0 if no target characters were found
 */
str_size_t str_trim_characters(char* string, const char* targets);

//str_size_t str_copy_trim_characters(const char* const source, destination, const char* targets_array);

/**
 * @brief Normalizes and trims all whitespace in a string.
 *
 * @param[in,out] string The string to process (modified in-place)
 * @return Number of characters removed/trimmed (str_size_t)
 *
 * @details Performs three operations:
 *          1. Converts all whitespace characters (STR_WHITESPACE) to standard spaces (0x20)
 *          2. Trims leading and trailing spaces
 *          3. Collapses consecutive spaces between words to single spaces
 *
 * @pre string != NULL
 *
 * @warning Modifies the string in-place
 * @warning Uses STR_WHITESPACE definition rather than locale-dependent isblank()
 *
 * @note More comprehensive than standard isblank() which only handles:
 *       - Space (0x20)
 *       - Horizontal tab (0x09)
 * @note Preserves null-termination
 * @note Returns 0 if no whitespace needed trimming
 */
inline str_size_t str_trim_whitespace(char* string) {
    return str_trim_characters(string, STR_WHITESPACE);
}

//str_size_t str_copy_trim_whitespace(const char* source, char* destination);

//str_size_t str_remove_characters(char* source, const char* targets);

/**
 * @brief Removes all punctuation characters from a string in-place.
 *
 * @param[in,out] string Null-terminated string to modify (must not be NULL)
 * @return Number of punctuation characters removed (str_size_t)
 *
 * @details Removes all characters for which ispunct() returns true, including:
 *          - ! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ ` { | } ~
 *          The operation is performed in O(n) time with O(1) space complexity.
 *
 * @pre string != NULL (checked by assert)
 *
 * @warning Modifies the input string directly
 * @warning Behavior depends on locale settings for ispunct()
 * @warning Not thread-safe during modification
 *
 * @note Preserves the order of remaining characters
 * @note Always maintains proper null-termination
 * @note Returns 0 if no punctuation was found
 * @note Differs from str_remove_character() which removes specific characters
 */
str_size_t str_remove_punctuation(char* string);

//str_size_t str_copy_remove_characters(const char* source, char* destination, const char* targets);

#endif

/** @} */ // end of string_scrubbing group

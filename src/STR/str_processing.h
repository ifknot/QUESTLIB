/**
 * @file str_processing.h
 * @brief String conversion utilities
 *
 * @defgroup string_processing String Processing Functions
 * @{
 */

#ifndef STR_PROCESSING_H
#define STR_PROCESSING_H

#include <stdint.h>

#include "str_constants.h"
#include "str_types.h"

/**
 * @brief Converts a string to uppercase in-place.
 *
 * @param[in,out] string Null-terminated string to convert (must not be NULL)
 * @return Pointer to the modified string (same as input)
 *
 * @details Modifies the original string by converting each character to uppercase
 * using the standard toupper() function. The conversion is locale-sensitive.
 *
 * @pre string != NULL (checked by assert)
 * @warning Modifies the input string directly
 * @note For empty strings, the function returns immediately
 */
char* str_to_upper_case(char* string);

/**
 * @brief Copies a string while converting characters to uppercase.
 *
 * @param[in] source Null-terminated source string (must not be NULL)
 * @param[out] destination Buffer to store uppercase copy (must have sufficient space)
 * @return Pointer to the destination buffer
 *
 * @details Copies characters from source to destination while converting each
 * character to uppercase using toupper(). The destination buffer must be large
 * enough to hold the entire converted string including null-terminator.
 *
 * @pre source != NULL (checked by assert)
 * @pre destination != NULL (checked by assert)
 * @pre destination buffer must be at least (strlen(source) + 1) bytes
 *
 * @warning Does not perform bounds checking on destination buffer
 * @warning Source and destination buffers must not overlap
 *
 * @note For empty strings, only the null-terminator is copied
 * @note Uses the current locale's uppercase conversion rules
 */
//char* str_copy_to_upper_case(const char* source, char* destination);

/**
 * @brief Converts a string to lowercase in-place.
 *
 * @param[in,out] string Null-terminated string to convert (must not be NULL)
 * @return Pointer to the modified string (same as input)
 *
 * @details Modifies the original string by converting each character to lowercase
 * using the standard tolower() function. The conversion is locale-sensitive.
 *
 * @pre string != NULL (checked by assert)
 * @warning Modifies the input string directly (not thread-safe)
 * @note For empty strings, the function returns immediately
 */
char* str_to_lower_case(char* string);

/**
 * @brief Copies a string while converting characters to lowercase.
 *
 * @param[in] source Null-terminated source string (must not be NULL)
 * @param[out] destination Buffer to store lowercase copy (must have sufficient space)
 * @return Pointer to the destination buffer
 *
 * @details Copies characters from source to destination while converting each
 * character to lowercase using tolower(). The destination buffer must be large
 * enough to hold the entire converted string including null-terminator.
 *
 * @pre source != NULL (checked by assert)
 * @pre destination != NULL (checked by assert)
 * @pre destination buffer must be at least (strlen(source) + 1) bytes
 *
 * @warning Does not perform bounds checking on destination buffer
 * @warning Source and destination buffers must not overlap
 *
 * @note For empty strings, only the null-terminator is copied
 * @note Uses the current locale's uppercase conversion rules
 */
//char* str_copy_to_lower_case(const char* source, char* destination);

/**
 * @brief Counts the number of distinct alpha-numeric locale specific words in a string.
 *
 * @param[in] string Null-terminated string to analyze (must not be NULL)
 * @return Number of words found (str_size_t)
 *
 * @details A word is defined as a sequence of alphanumeric characters,
 *          allowing for apostrophes and hyphens within words (e.g., "don't", "state-of-the-art").
 *          Words containing numbers (e.g., "C3PO") are counted as single words.
 *
 * @pre string != NULL (checked by assert)
 *
 * @note Leading/trailing non-alphanumeric characters are ignored
 * @note Consecutive non-word characters between words are treated as single separator
 * @note Empty strings return 0
 * @note The function is locale-sensitive for character classification
*/
str_size_t str_count_words(const char* string);

/**
 * @brief Counts the number of lines in a string.
 *
 * @param[in] string Null-terminated string to analyze (must not be NULL)
 * @return Number of lines (str_size_t)
 *
 * @details Counts newline characters ('\n') in the string and returns:
 *          - 0 for empty string
 *          - 1 for string with no newlines
 *          - n+1 for string with n newlines
 *
 * @pre string != NULL (checked by assert)
 *
 * @note A single trailing newline does not count as an additional line
 * @note Windows-style CRLF ("\r\n") sequences are counted as single newlines
 * @note Empty lines between text (consecutive newlines) are counted
*/
str_size_t str_count_lines(const char* string);

/**
 * @brief Extracts the next word from a string at a given position.
 *
 * @param[in] string Input string to read from (must not be NULL)
 * @param[in,out] i Iterator tracking current position in string
 * @param[out] word Buffer to store the extracted word
 * @param[in] word_size Maximum capacity of the word buffer
 * @return Number of characters written to word buffer (excluding null-terminator)
 *
 * @details Reads the next word starting from position *i, where a word is defined as:
 *          - Beginning with an alphabetic character (isalpha())
 *          - May contain alphanumerics, apostrophes, and hyphens
 *          - Stops at first non-word character or buffer capacity
 *
 * @pre string != NULL (checked by assert)
 * @pre word != NULL (checked by assert)
 * @pre word_size > 0 (checked by assert)
 * @pre i != NULL (not checked)
 *
 * @warning No bounds checking on iterator *i (must be valid for string)
 * @warning Caller must ensure word buffer is large enough for expected words
 * @warning Modifies both the iterator position and word buffer
 *
 * @note Always null-terminates the output word
 * @note Returns 0 if no word found before string end
 * @note Iterator *i is advanced past the word and any following non-word chars
 * @note Hyphenated words and contractions are treated as single words
 */
str_size_t str_read_word(const char* string, str_iterator_t* i, char* word, const str_size_t word_size);

/**
 * @brief Reads a line from a string at the current iterator position.
 *
 * @param[in] string Input string to read from (must not be NULL)
 * @param[in,out] i Iterator tracking current read position
 * @param[out] line Buffer to store the extracted line
 * @param[in] line_size Maximum capacity of the line buffer
 * @return Number of characters written (excluding null-terminator)
 *
 * @details Reads characters starting from position *i until:
 *          - A newline character ('\n') is encountered
 *          - The end of string is reached
 *          - The line buffer is filled (line_size-1 characters)
 *          The newline character itself is not included in the output.
 *
 * @pre string != NULL (checked by assert)
 * @pre line != NULL (checked by assert)
 * @pre line_size > 0 (checked by assert)
 * @pre i != NULL (not checked)
 *
 * @warning No bounds checking on iterator *i (must be valid for string)
 * @warning Does not handle CRLF ("\r\n") line endings specially
 * @warning Caller must manage iterator lifetime and validity
 *
 * @note Always null-terminates the output buffer
 * @note Returns 0 if already at end of string
 * @note Iterator *i is advanced to position after the newline or end of string
 * @note Safe for empty lines (returns 0 but advances past newline)
 */
str_size_t str_read_line(const char* string, str_iterator_t* i, char* line, const str_size_t line_size);

/**
 * @brief Enumerates all words in a string into an array of strings.
 *
 * @param[in] string Input string to process (must not be NULL)
 * @param[out] string_array Pre-allocated array of string buffers
 * @param[in] array_size Maximum number of words to extract (size of array)
 * @param[in] word_size Size of each individual word buffer
 * @return Number of words actually extracted (str_size_t)
 *
 * @details Extracts words from the string using str_read_word() until:
 *          - The entire string is processed
 *          - The string array is filled
 *          Each word buffer is null-terminated and contains only one word.
 *
 * @pre string != NULL (checked by assert)
 * @pre string_array != NULL (checked by assert)
 * @pre array_size > 0 (checked by assert)
 * @pre word_size > 0 (checked by assert)
 * @pre Each string_array element must point to allocated memory of size word_size
 *
 * @warning No bounds checking on string_array elements
 * @warning Potential buffer overflow if word_size is too small for any word
 * @warning Modifies both the input string_array and its contents
 *
 * @note Returns actual word count (may be less than array_size)
 * @note Empty string returns 0
 * @note Follows same word definition as str_read_word() (alphanumeric + apostrophes/hyphens)
 */
str_size_t str_enumerate_words(const char* string, char** string_array, const str_size_t array_size, const str_size_t word_size);

/**
 * @brief Comparison function for qsort to sort strings alphabetically
 * @param a Pointer to first string (cast from void*)
 * @param b Pointer to second string (cast from void*)
 * @return <0 if a < b, 0 if equal, >0 if a > b (per strcmp semantics)
 *
 * @details Case-sensitive comparison using standard strcmp().
 *          Designed specifically for use with qsort() on char* arrays.
 *
 * @note This is a comparator function following qsort's required signature
 */
int str_compare_strings(const void *a, const void *b);

/**
 * @brief Sorts an array of strings in ascending alphabetical order
 * @param[in,out] string_array Array of null-terminated strings to sort
 * @param[in] array_size Number of elements in the array
 *
 * @details Performs case-sensitive sort using the system's qsort() with these properties:
 *          - In-place O(n log n) sort (modifies original array)
 *          - Lexicographical order using strcmp() semantics
 *          - Handles all valid ASCII/UTF-8 strings
 *          - Sorts NULL pointers safely if present
 *
 * @pre string_array != NULL (asserted)
 * @pre array_size > 0 (asserted)
 * @pre Each element points to valid memory (not verified)
 *
 * @warning Modifies the input array directly
 * @warning Not a stable sort (equal elements may change order)
 * @warning Case-sensitive ("Zebra" comes before "apple")
 */
void str_sort_words(char** string_array, const str_size_t array_size);

/**
 * @brief Case-insensitive string comparison function for qsort
 * @param a Pointer to first string (cast from void*)
 * @param b Pointer to second string (cast from void*)
 * @return <0 if a < b, 0 if equal, >0 if a > b (per strcasecmp semantics)
 *
 * @details Case-insensitive comparison using strcasecmp().
 *          Compares strings alphabetically ignoring case differences.
 *          Example: "apple" == "Apple", "zebra" > "Apple"
 *
 * @note This is a comparator function following qsort's required signature
 * @see str_sort_words_case_insensitive()
 * @see strcmp() (for case-sensitive version)
 */
int str_compare_strings_case_insensitive(const void *a, const void *b);

/**
 * @brief Sorts an array of strings alphabetically (case-insensitive)
 * @param[in,out] string_array Array of null-terminated strings to sort
 * @param[in] array_size Number of strings in the array
 *
 * @details Uses qsort() with strcasecmp() to perform:
 *          - In-place case-insensitive sorting
 *          - O(n log n) time complexity
 *          - "Apple" will sort next to "apple"
 *
 * @pre string_array != NULL (asserted)
 * @pre array_size > 0 (asserted)
 * @pre Each string_array element must point to valid null-terminated string
 *
 * @warning Modifies the input array directly
 * @warning Not stable sort (order of equal elements may change)
 * @warning Locale-dependent behavior (strcasecmp)
 *
 * @see str_compare_strings_case_insensitive()
 * @see str_sort_words() (for case-sensitive version)
 * @see qsort()
 */
void str_sort_words_case_insensitive(char** string_array, const str_size_t array_size);

#endif

/** @} */ // end of string_processing group

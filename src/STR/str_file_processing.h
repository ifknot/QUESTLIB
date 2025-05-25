/**
 * @file str_file_processing.h
 * @brief File-based string processing utilities
 *
 * @defgroup file_processing File String Processing
 * @{
 */

#ifndef STR_FILE_PROCESSING_H
#define STR_FILE_PROCESSING_H

#include <stdint.h>

#include "../DOS/dos_services_files.h"

#include "str_types.h"

/**
 * @brief Counts words in a file
 * @param fhandle Valid DOS file handle
 * @return Number of words (str_size_t)
 *
 * @details Word definition:
 *          - Starts with alphabetic character (isalpha())
 *          - May contain alphanumerics, apostrophes, and hyphens
 *          - Counts from current file position
 *          - Restores original file position
 *
 * @pre fhandle != NULL (checked by assert)
 *
 * @warning Locale-dependent for character classification
 * @note Handles compound words and contractions
 * @see str_file_read_word()
 */
str_size_t str_file_count_words(const dos_file_handle_t fhandle);

/**
 * @brief Counts lines in a file
 * @param fhandle Valid DOS file handle
 * @return Number of lines (str_size_t)
 *
 * @details Counts newline characters ('\n') in file:
 *          - Starts counting from current file position
 *          - Restores original file position before returning
 *          - Empty files return 0
 *          - Files without trailing newline count correctly
 *
 * @pre fhandle != NULL (checked by assert)
 *
 * @warning Modifies file position during operation (but restores it)
 */
str_size_t str_file_count_lines(const dos_file_handle_t fhandle);

/**
 * @brief Reads a single character from a file
 *
 * @param fhandle Valid DOS file handle to read from
 * @param[out] chr Pointer to store the read character
 * @return 1 if character was read successfully, 0 on EOF/error
 *
 * @details Wrapper around dos_read_file() that:
 *          - Reads exactly one character
 *          - Advances the file position pointer
 *          - Provides simple EOF detection (return value)
 *
 * @pre fhandle != NULL
 * @pre chr != NULL
 *
 * @note Equivalent to fgetc() but for DOS file handles
 * @note Return value matches DOS convention (1=success, 0=failure)
 */
inline str_size_t str_file_read_char(const dos_file_handle_t fhandle, char* chr) {
    return dos_read_file(fhandle, chr, 1);
}

/**
 * @brief Reads next word from file
 * @param fhandle Valid DOS file handle
 * @param[out] word Buffer to store word
 * @param word_size Capacity of word buffer
 * @return Number of characters read (excluding null-terminator)
 *
 * @details Reads until:
 *          - Word buffer is full
 *          - Invalid word character encountered
 *          - EOF reached
 *          Always null-terminates output
 *
 * @pre All parameters != NULL (checked by assert)
 * @pre word_size > 0
 *
 * @warning No buffer overflow protection
 * @note Advances file position
 * @see str_file_count_words()
 */
str_size_t str_file_read_word(const dos_file_handle_t fhandle, char* word, const str_size_t word_size);

/**
 * @brief Reads next line from file
 * @param fhandle Valid DOS file handle
 * @param[out] line Buffer to store line
 * @param line_size Capacity of line buffer
 * @return Number of characters read (excluding null-terminator)
 *
 * @details Reads until:
 *          - Newline encountered
 *          - Line buffer is full
 *          - EOF reached
 *          Does not include newline in output
 *
 * @pre All parameters != NULL (checked by assert)
 * @pre line_size > 0
 *
 * @warning No buffer overflow protection
 * @note Advances file position
 */
str_size_t str_file_read_line(const dos_file_handle_t fhandle, char* line, const str_size_t line_size);

/**
 * @brief Reads all words from a file into an array
 * @param path_name Path to file
 * @param[out] string_array Pre-allocated array of buffers
 * @param array_size Number of buffers in array
 * @param word_size Capacity of each word buffer
 * @return Number of words actually read
 *
 * @details Combines file operations:
 *          - Opens/closes file automatically
 *          - Reads until array is full or EOF
 *          - Uses same word definition as str_file_read_word()
 *
 * @pre All parameters != NULL (checked by assert)
 * @pre array_size > 0
 * @pre word_size > 0
 *
 * @warning Requires pre-allocated buffers
 */
str_size_t str_file_enumerate_words(const char * path_name, char** string_array, const str_size_t array_size, const str_size_t word_size);

#endif

/** @} */ // end of file_processing group

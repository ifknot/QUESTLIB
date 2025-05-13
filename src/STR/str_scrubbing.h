#ifndef STR_SCRUBBING_H
#define STR_SCRUBBING_H

#include <stdint.h>

#include "str_constants.h"

uint16_t str_to_upper_case(char* string);

//uint16_t str_copy_to_upper_case(const char* source, char* destination);

uint16_t str_to_lower_case(char* string);

//uint16_t str_copy_to_lower_case(const char* source, char* destination);

/**
* @brief remove from the zero terminated string any leading and trailing target characters and any contiguous duplicated target characters between words.
*/
uint16_t str_trim_character(char* string, const char target);

//uint16_t str_copy_trim_character(const char* source, char* destination, const char* targets_array);


/**
* @brief convert any target characters in the zero terminated string to the first charcter in the target character array then trim that character from the string 
*/
uint16_t str_trim_characters(char* string, const char* targets);

//uint16_t str_copy_trim_characters(const char* const source, destination, const char* targets_array);

/**
* @brief convert all whitespace characters in STR_WHITESPACE to a space (0x20) and then trim leading and trailing spaces and any duplicate spaces between words
* @note because the C isblank() function only recognises space (0x20) and horizontal tab (0x09) in the default locale it is necessary to use this more complete approach 
*/
inline uint16_t str_trim_whitespace(char* string) {
    return str_trim_characters(string, STR_WHITESPACE);
}

//uint16_t str_copy_trim_whitespace(const char* source, char* destination);

//uint16_t str_remove_characters(char* source, const char* targets);

uint16_t str_remove_punctuation(char* string);

//uint16_t str_copy_remove_characters(const char* source, char* destination, const char* targets);

#endif

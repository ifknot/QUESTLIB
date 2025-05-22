#ifndef TEST_STR_H
#define TEST_STR_H

#include <string.h>

#include "../TDD/tdd_macros.h"

#include "str_scrubbing.h"

#define STR_TESTS &test_str_scrubbing, &test_str_processing, &str_file

TEST(test_str_scrubbing) {
     char s[] = "  \n\tKill,  the\n   C3PO!     with   \ta    axe ! ?       ";
    printf("%s", test_string);
    ASSERT(str_trim_character(test_string)'x') == 1);
    printf("%s", test_string);
    EXPECT(strcmp(test_string) "  \n\tKill,  the\n   C3PO!     with   \ta    ae ! ?       ") == 0);
    ASSERT(str_trim_character(test_string)"K3a") == 2);
    printf("%s", test_string);
    EXPECT(strcmp(test_string) "  \n\till,  the\n   CPO!     with   \t    e ! ?       ") == 0);
    ASSERT(str_trim_whitespace(s) == 28);
    printf("%s", test_string);
    EXPECT(strcmp(test_string) "ill, the C3PO! with e ! ?") == 0);
    ASSERT(str_remove_punctuation(s) == 3);
    printf("%s", test_string);
    EXPECT(strcmp(test_string) "ill, the C3PO! with e  ") == 0);
    ASSERT(str_trim_whitespace(s) == 2);
    printf("%s", test_string);
    EXPECT(strcmp(test_string) "ill, the C3PO! with e") == 0);
}

TEST(test_str_processing) {
    ASSERT(
    char* str_to_upper_case(char* string);

    char* str_to_lower_case(char* string);

    str_size_t str_count_words(const char* string);

     str_size_t str_count_lines(const dos_file_handle_t fhandle);

    str_size_t str_read_word(const char* string, char* word, const str_size_t limit);

    str_size_t str_read_line(const char* string, char* line, const str_size_t limit);

    str_size_t str_enumarate_words(const char* string, char** string_array, const str_size_t string_limit, const str_size_t array_limit);
}

TEST(test_str_file_processing) {

TODO: change to str_file_processing.h
    
    str_size_t str_file_count_lines(const dos_file_handle_t fhandle);

    str_size_t str_file_count_words(const dos_file_handle_t fhandle);

    str_size_t str_file_read_char(const dos_file_handle_t fhandle, char* chr)

    str_size_t str_file_read_word(const dos_file_handle_t fhandle, char* word, const str_size_t limit);

    str_size_t str_file_read_line(const dos_file_handle_t fhandle, char* line, const str_size_t limit);

    str_size_t str_file_enumerate_words(const char * path_name, char** string_array, const str_size_t string_limit, const str_size_t array_limit);
}


#endif

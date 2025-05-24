#ifndef TEST_STR_H
#define TEST_STR_H

#include <string.h>

#include "../TDD/tdd_macros.h"

#include "str_scrubbing.h"
#include "str_processing.h"

#define STR_TESTS &test_str_scrubbing, &test_str_processing, &test_str_file_processing

TEST(test_str_scrubbing) {
    char test_string[] = "  \n\tKill,  the\n   QC3PO!     with   \ta    axe ! ?       ";
    V(printf("->%s<-\n", test_string););
    ASSERT(str_remove_character(test_string, 'Q') == 1);
    V(printf("->%s<-\n", test_string););
    EXPECT(str_trim_character(test_string, ' ') == 21);
    EXPECT(strcmp(test_string, "\n\tKill, the\n C3PO! with \ta axe ! ?") == 0);
    V(printf("->%s<-\n", test_string););
    EXPECT(str_trim_characters(test_string, "Kl") == 1);
    V(printf("->%s<-\n", test_string););
    EXPECT(strcmp(test_string, "\n\tKiK, the\n C3PO! with \ta axe ! ?") == 0);
    ASSERT(str_trim_whitespace(test_string) == 4);
    V(printf("->%s<-\n", test_string););
    EXPECT(strcmp(test_string, "KiK, the C3PO! with a axe ! ?") == 0);
    EXPECT(str_remove_punctuation(test_string) == 4);
    V(printf("->%s<-\n", test_string););
}

TEST(test_str_processing) {
    /*
    str_iterator_t i = 0;
    char test_string[] = "The Quick Brown fox jumps over the lazy dog.\nThis sentence uses all 26 letters of the alphabet, making it useful for testing typewriters, keyboards, and fonts.\nIt's also commonly used for touch-typing practice.";
    char test_word[15];
    char test_line[127];
    char test_array[9][15];
    memset(test_array, 0, sizeof(test_array));
    printf("%s\n", test_string);
    ASSERT(strcmp(str_to_lower_case(test_string), "the quick brown fox jumps over the lazy dog.\nthis sentence uses all 26 letters of the alphabet, making it useful for testing typewriters, keyboards, and fonts.\nit's also commonly used for touch-typing practice.") == 0);
    printf("%s\n", test_string);
    ASSERT(strcmp(str_to_upper_case(test_string), "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\nTHIS SENTENCE USES ALL 26 LETTERS OF THE ALPHABET, MAKING IT USEFUL FOR TESTING TYPEWRITERS, KEYBOARDS, AND FONTS.\nIT'S ALSO COMMONLY USED FOR TOUCH-TYPING PRACTICE.") == 0);
    printf("%s\n", test_string);
    ASSERT(str_count_words(test_string) == 34);
    //ASSERT(str_count_lines(test_string) == 3);
    ASSERT(str_read_word(test_string, &i, test_word, sizeof(test_word)) == 3);
    EXPECT(strcmp(test_word, "THE") == 0);
    ASSERT(str_read_word(test_string, &i, test_word, sizeof(test_word)) == 5);
    EXPECT(strcmp(test_word, "QUICK") == 0);
    ASSERT(str_read_line(test_string, &i, test_line, sizeof(test_line)) == 34);
    EXPECT(strcmp(test_line, "BROWN FOX JUMPS OVER THE LAZY DOG."));
    i = 0;
   /*  ASSERT(str_enumarate_words(test_string, test_array, 15, 9) == 9);
    i = 0;
    for(int j = 0; j < 9; ++j) {
        str_read_word(test_string, test_word, &i, sizeof(test_word));
        ASSERT(strcmp(test_word, test_array[j]) == 0);
    }*/
}

TEST(test_str_file_processing) {

/*

    str_size_t str_file_count_lines(const dos_file_handle_t fhandle);

    str_size_t str_file_count_words(const dos_file_handle_t fhandle);

    str_size_t str_file_read_char(const dos_file_handle_t fhandle, char* chr)

    str_size_t str_file_read_word(const dos_file_handle_t fhandle, char* word, const str_size_t limit);

    str_size_t str_file_read_line(const dos_file_handle_t fhandle, char* line, const str_size_t limit);

    str_size_t str_file_enumerate_words(const char * path_name, char** string_array, const str_size_t string_limit, const str_size_t array_limit);
*/
}


#endif

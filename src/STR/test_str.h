/**
 * @file test_str.h
 * @brief Comprehensive tests for string utilities library
 * @defgroup string_tests String Library Tests
 * @{
 */
#ifndef TEST_STR_H
#define TEST_STR_H

#include <string.h>

#include "../TDD/tdd_macros.h"
#include "../DOS/dos_services_files.h"

#include "str_headers.h"

/// @brief Array of all test cases for the string library
#define STR_TESTS &test_str_memory, &test_str_scrubbing, &test_str_processing, &test_str_file_processing

/* ----------------- Memory Allocation Tests ----------------- */

/**
 * @test Verify string memory allocation functions
 * @covers str_make_string(), str_make_copy(), str_make_string_array()
 */
TEST(test_str_memory) {
    tdd_verbose = false;
    mem_arena_t* arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_1K);

    // Verify arena
    ASSERT(mem_arena_capacity(arena) == MEM_SIZE_1K);
    ASSERT(mem_arena_size(arena) == mem_arena_capacity(arena));

    // Basic string creation
    char* s1 = str_make_string(arena, 10);
    ASSERT(s1 != NULL);
    ASSERT(strlen(s1) == 0); // Null-terminated empty string
    V(printf("->%s<-\n", s1););

    // String copying
    char* s2 = str_make_copy(arena, "hello world");
    ASSERT(strcmp(s2, "hello world") == 0);
    V(printf("->%s<-\n", s2););

    // String array creation
    char** ss = str_make_string_array(arena, 3, 16);
    ASSERT(ss != NULL);
    V(printf("size = %li\n", mem_arena_size(arena)););

    // Verify arena accounting
    ASSERT(mem_arena_size(arena) == MEM_SIZE_1K - 10 - 12 - (3 * 4) - (16 * 3));
    mem_arena_delete(arena);
}

/* ----------------- Text Cleaning Tests ----------------- */

/**
 * @test Validate string scrubbing operations
 * @covers str_remove_character(), str_trim_character(),
 *        str_trim_characters(), str_trim_whitespace(), str_remove_punctuation()
 */
TEST(test_str_scrubbing) {
    tdd_verbose = false;
    char test_string[] = "  \n\tKill,  the\n   QC3PO!     with   \ta    axe ! ?       ";
    V(printf("->%s<-\n", test_string););

    // Character removal
    ASSERT(str_remove_character(test_string, 'Q') == 1);
    ASSERT(strstr(test_string, "C3PO") != NULL);
    V(printf("->%s<-\n", test_string););

    // Trimming tests
    ASSERT(str_trim_character(test_string, ' ') == 21);
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

/* ----------------- Core Processing Tests ----------------- */

/**
 * @test Exercise core string processing functions
 * @covers str_to_upper_case(), str_to_lower_case(),
 *        str_count_words(), str_count_lines(),
 *        str_read_word(), str_read_line(), str_enumerate_words()
 */
TEST(test_str_processing) {
    tdd_verbose = false;
    mem_arena_t* arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_1K);
    str_iterator_t i = 0;
    char test_string[] = "The Quick Brown fox jumps over the lazy dog.\nThis sentence uses all 26 letters of the alphabet, making it useful for testing typewriters, keyboards, and fonts.\nIt's also commonly used for touch-typing practice.";
    char* test_word = str_make_string(arena, STR_MAX_WORD_LENGTH);
    char* test_line = str_make_string(arena, 128);
    char** test_array = str_make_string_array(arena, 9, STR_MAX_WORD_LENGTH);
    V(printf("%s\n", test_string););

    // Case conversion
    ASSERT(strcmp(str_to_upper_case(test_string), "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\nTHIS SENTENCE USES ALL 26 LETTERS OF THE ALPHABET, MAKING IT USEFUL FOR TESTING TYPEWRITERS, KEYBOARDS, AND FONTS.\nIT'S ALSO COMMONLY USED FOR TOUCH-TYPING PRACTICE.") == 0);
    V(printf("%s\n", test_string););
    ASSERT(strcmp(str_to_lower_case(test_string), "the quick brown fox jumps over the lazy dog.\nthis sentence uses all 26 letters of the alphabet, making it useful for testing typewriters, keyboards, and fonts.\nit's also commonly used for touch-typing practice.") == 0);
    V(printf("%s\n", test_string););

    // Word and line counting
    ASSERT(str_count_words(test_string) == 34);
    ASSERT(str_count_lines(test_string) == 3);

    // Word and line reading
    EXPECT(str_read_word(test_string, &i, test_word, STR_MAX_WORD_LENGTH) == 3);
    V(printf("%s\n",test_word););
    EXPECT(strcmp(test_word, "the") == 0);
    EXPECT(str_read_word(test_string, &i, test_word, STR_MAX_WORD_LENGTH) == 5);
    V(printf("%s\n",test_word););
    EXPECT(strcmp(test_word, "quick") == 0);
    EXPECT(str_read_line(test_string, &i, test_line, 128) == 35);
    V(printf("%s\n", test_line););
    EXPECT(strcmp(test_line, "brown fox jumps over the lazy dog."));

    // Word enumerating and sorting
    EXPECT(str_enumerate_words(test_line, test_array, 9, STR_MAX_WORD_LENGTH) == 7);
    V(for(int i =0; i < 9; ++i) printf("%s ", test_array[i]););
    str_sort_words(test_array, 9);
    V(for(int i =0; i < 9; ++i) printf("%s ", test_array[i]););

    V(printf("\nbytes used %i\n", mem_arena_used(arena)););
    V(printf("bytes spare %i\n", mem_arena_size(arena)););
    mem_arena_delete(arena);
}

/* ----------------- File Handling Tests ----------------- */

/**
 * @test Validate file-based string operations
 * @covers str_file_count_words(), str_file_count_lines(),
 *        str_file_read_char(), str_file_read_word(),
 *        str_file_read_line(), str_file_enumerate_words()
 */
TEST(test_str_file_processing) {
    tdd_verbose = false;
    mem_arena_t* arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_4K);
    char c = ' ';
    char s[15] = {0};
    char l[40] = {0};
    char** test_array = str_make_string_array(arena, 92, STR_MAX_WORD_LENGTH);
    dos_file_handle_t f = dos_open_file("assets/poem.txt", ACCESS_READ_ONLY);

    // Line/word counting
    V(printf("words %li\n",str_file_count_words(f)););
    ASSERT(str_file_count_words(f) == 90);
    V(printf("lines %li\n",str_file_count_lines(f)););
    ASSERT(str_file_count_lines(f) == 17);

    // Content reading
    ASSERT(str_file_read_char(f, &c) == 1);
    EXPECT(c == 'T');
    V(printf("%c\n",c););
    ASSERT(str_file_read_char(f, &c) == 1);
    EXPECT(c == 'h');
    V(printf("%c\n",c););
    EXPECT(str_file_read_word(f, s, 15) == 2);
    EXPECT(strcmp(s, "is") == 0);
    V(printf("%s\n",s););
    EXPECT(str_file_read_line(f, l, 40) == 12);
    EXPECT(strcmp(l, "Be The Verse") == 0);
    V(printf("->%s<-\n",l););

    // String enumeration from file and sorting
    EXPECT(str_file_enumerate_words("assets/poem.txt", test_array, 92, STR_MAX_WORD_LENGTH) == 90);
    str_sort_words_case_insensitive(test_array, 92);
    V(for(int i =0; i < 92; ++i) printf("%s ", test_array[i]););
    V(printf("\nbytes used %i\n", mem_arena_used(arena)););
    V(printf("bytes spare %i\n", mem_arena_size(arena)););

    dos_close_file(f);
    mem_arena_delete(arena);
}

#endif

/** @} */ // end of string_tests group

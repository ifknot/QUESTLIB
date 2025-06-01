/**
 * @file test_parse_dictionary.h
 * @brief Test-driven development for parse dictionary
 * @defgroup dictionary_tests Parse Dictionary Tests
 * @{
 */
#ifndef TEST_PARSE_DICTIONARY_H
#define TEST_PARSE_DICTIONARY_H

#include <stdio.h>
#include <assert.h>
#include "parse_dictionary.h"
#include "../TDD/tdd_macros.h"

/// @brief Array of all test cases for the dictionary
#define DICTIONARY_TESTS &test_empty_dict, &test_single_element, \
                         &test_first_element_search, &test_last_element_search, \
                         &test_midway_search, &test_not_found, \
                         &test_add_remove, &test_sorting, \
                         &test_capacity_limits

#define TEST_DICT_CAPACITY 16  // Test dictionary capacity

/* ----------------- Test Fixtures ----------------- */

static parse_dictionary_t* test_dict = NULL;
static mem_arena_t* test_arena = NULL;

void setup() {
    test_arena = mem_arena_new(MEM_ARENA_POLICY_DOS, MEM_SIZE_1K);
    test_dict = parse_dictionary_create(test_arena, TEST_DICT_CAPACITY);
}

void teardown() {
    if (test_arena) {
        mem_arena_delete(test_arena);
        test_arena = NULL;
        test_dict = NULL;  // Dictionary is part of the arena
    }
}

/* ----------------- Helper Functions ----------------- */

static void populate_test_dictionary() {
    parse_dictionary_add(test_dict, "apple", 1);
    parse_dictionary_add(test_dict, "banana", 2);
    parse_dictionary_add(test_dict, "cherry", 3);
    parse_dictionary_add(test_dict, "date", 4);
    parse_dictionary_add(test_dict, "elderberry", 5);
    parse_dictionary_sort(test_dict);
}

/* ----------------- Core Test Cases ----------------- */

TEST(test_empty_dict) {
    setup();

    ASSERT(parse_dictionary_is_empty(test_dict));
    ASSERT(parse_dictionary_size(test_dict) == 0);
    ASSERT(parse_dictionary_search(test_dict, "anything") == PARSE_DICTIONARY_EMPTY);

    teardown();
}

TEST(test_single_element) {
    setup();

    parse_dictionary_add(test_dict, "single", 99);
    parse_dictionary_sort(test_dict);

    ASSERT(parse_dictionary_size(test_dict) == 1);
    ASSERT(parse_dictionary_search(test_dict, "single") == 0);
    ASSERT(parse_dictionary_at(test_dict, 0).token == 99);
    ASSERT(parse_dictionary_search(test_dict, "nonexistent") == PARSE_DICTIONARY_NOT_FOUND);

    teardown();
}

TEST(test_first_element_search) {
    setup();
    populate_test_dictionary();

    // "apple" should be first after sorting
    int index = parse_dictionary_search(test_dict, "apple");
    ASSERT(index == 0);
    ASSERT(parse_dictionary_at(test_dict, index).token == 1);

    teardown();
}

TEST(test_last_element_search) {
    setup();
    populate_test_dictionary();

    // "elderberry" should be last after sorting
    int index = parse_dictionary_search(test_dict, "elderberry");
    ASSERT(index == parse_dictionary_size(test_dict) - 1);
    ASSERT(parse_dictionary_at(test_dict, index).token == 5);

    teardown();
}

TEST(test_midway_search) {
    setup();
    populate_test_dictionary();

    // "cherry" should be in the middle
    int index = parse_dictionary_search(test_dict, "cherry");
    ASSERT(index > 0 && index < parse_dictionary_size(test_dict) - 1);
    ASSERT(parse_dictionary_at(test_dict, index).token == 3);

    teardown();
}

TEST(test_not_found) {
    setup();
    populate_test_dictionary();

    // "fig" not in dictionary
    ASSERT(parse_dictionary_search(test_dict, "fig") == PARSE_DICTIONARY_NOT_FOUND);

    teardown();
}

/* ----------------- Extended Test Cases ----------------- */

TEST(test_add_remove) {
    setup();

    // Add elements
    ASSERT(parse_dictionary_add(test_dict, "zebra", 10) >= 0);
    ASSERT(parse_dictionary_add(test_dict, "aardvark", 11) >= 0);
    ASSERT(parse_dictionary_size(test_dict) == 2);

    // Sort and verify order
    parse_dictionary_sort(test_dict);
    ASSERT(parse_dictionary_search(test_dict, "aardvark") == 0);
    ASSERT(parse_dictionary_search(test_dict, "zebra") == 1);

    // Remove and verify
    ASSERT(parse_dictionary_remove(test_dict, 0) == 1);
    ASSERT(parse_dictionary_size(test_dict) == 1);
    ASSERT(parse_dictionary_search(test_dict, "zebra") == 0);

    teardown();
}

TEST(test_sorting) {
    setup();

    // Add out-of-order elements
    parse_dictionary_add(test_dict, "mango", 20);
    parse_dictionary_add(test_dict, "apricot", 21);

    // Verify unsorted search fails
    ASSERT(parse_dictionary_search(test_dict, "apricot") == PARSE_DICTIONARY_NOT_SORTED);

    // Sort and verify
    parse_dictionary_sort(test_dict);
    ASSERT(parse_dictionary_is_sorted(test_dict));
    ASSERT(parse_dictionary_search(test_dict, "apricot") == 0);
    ASSERT(parse_dictionary_search(test_dict, "mango") == 1);

    teardown();
}

TEST(test_capacity_limits) {
    setup();

    // Fill dictionary to capacity
    for (int i = 0; i < TEST_DICT_CAPACITY; i++) {
        char lexeme[16];
        snprintf(lexeme, sizeof(lexeme), "word%d", i);
        ASSERT(parse_dictionary_add(test_dict, lexeme, i) >= 0);
    }

    // Verify full
    ASSERT(parse_dictionary_is_full(test_dict));
    ASSERT(parse_dictionary_add(test_dict, "overflow", 99) == PARSE_DICTIONARY_FULL);

    // Remove one and add again
    ASSERT(parse_dictionary_remove(test_dict, 0) == TEST_DICT_CAPACITY - 1);
    ASSERT(parse_dictionary_add(test_dict, "newentry", 100) >= 0);

    teardown();
}

#endif

/** @} */ // end of dictionary_tests group

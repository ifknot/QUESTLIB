#ifndef TEST_FILE_H
#define TEST_FILE_H

/**
 * @file test_file_tools.c
 * @brief Test suite for file utilities
 * @defgroup file_tests File Tools Tests
 * @{
 */

#include <stdio.h>
#include <assert.h>
#include "file_tools.h"
#include "../TDD/tdd_macros.h"
#include "../DOS/dos_services_files.h"

#define FILE_TESTS &test_file_extension_normal, \
                   &test_file_extension_no_ext, \
                   &test_file_extension_edge_cases, \
                   &test_file_size_basic, \
                   &test_file_size_empty, \
                   &test_eof_detection, \
                   &test_invalid_handle_behavior

/* ----------------- Test Fixtures ----------------- */

static dos_file_handle_t test_file = 0;
static const char* TEST_FILE_PATH = "testfile.tmp";
static const char* TEST_CONTENTS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup_file() {
    // Create test file
    test_file = dos_create_file(TEST_FILE_PATH, ACCESS_READ_WRITE);
    if (test_file) {
        dos_write_file(test_file, TEST_CONTENTS, strlen(TEST_CONTENTS));
        dos_move_file_pointer(test_file, 0, FSEEK_SET);
    }
}

void teardown_file() {
    if (test_file) {
        dos_close_file(test_file);
        dos_delete_file(TEST_FILE_PATH);
    }
}

/* ----------------- Extension Tests ----------------- */

TEST(test_file_extension_normal) {
    const char* path = "test/document.txt";
    const char* ext = file_get_extension(path);
    ASSERT(ext != NULL);
    ASSERT(strcmp(ext, "txt") == 0);
}

TEST(test_file_extension_no_ext) {
    ASSERT(file_get_extension("README") == NULL);
    ASSERT(file_get_extension("/path/to/file") == NULL);
}

TEST(test_file_extension_edge_cases) {
    ASSERT(file_get_extension("") == NULL);
    ASSERT(file_get_extension(NULL) == NULL);
    ASSERT(file_get_extension("file.") == NULL);
    ASSERT(file_get_extension(".hidden") == NULL);
}

/* ----------------- File Size Tests ----------------- */

TEST(test_file_size_basic) {
    setup_file();

    file_size_t size = file_get_size(test_file);
    ASSERT(size == strlen(TEST_CONTENTS));

    teardown_file();
}

TEST(test_file_size_empty) {
    dos_file_handle_t f = dos_create_file("empty.tmp", ACCESS_READ_WRITE);
    ASSERT(f);

    ASSERT(file_get_size(f) == 0);

    dos_close_file(f);
    dos_delete_file("empty.tmp");
}

/* ----------------- EOF Detection Tests ----------------- */

TEST(test_eof_detection) {
    setup_file();

    // Start of file
    ASSERT(!file_position_indicator_is_eof(test_file));

    // Middle of file
    dos_move_file_pointer(test_file, 10, FSEEK_SET);
    ASSERT(!file_position_indicator_is_eof(test_file));

    // Exactly at EOF
    dos_move_file_pointer(test_file, 0, FSEEK_END);
    ASSERT(file_position_indicator_is_eof(test_file));

    teardown_file();
}

/* ----------------- Error Case Tests ----------------- */

TEST(test_invalid_handle_behavior) {
    ASSERT(file_get_size(0xFFFF) == 0);
    ASSERT(file_position_indicator_is_eof(0xFFFF) == true);
}

#endif

/** @} */ // end of file_tests group

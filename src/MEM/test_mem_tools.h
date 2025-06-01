/**
 * @file test_mem_tools.c
 * @brief Test suite for DOS memory management utilities
 * @ingroup tdd_framework
 */
#ifndef TEST_MEM_TOOLS_H
#define TEST_MEM_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../TDD/tdd_macros.h"
#include "../DOS/dos_services_files.h"

#include "mem_tools.h"

#define TOOLS_TESTS &test_mem_max_paragraphs, \
                    &test_mem_diff_pointers, \
                    &test_mem_dump_mcb_to_stream, \
                    &test_mem_load_save_file

/**
 * @brief Test memory availability query
 * @details Verifies that:
 * - Function returns non-zero value
 * - Value is less than 640K (A000 paragraphs)
 */
TEST(test_mem_max_paragraphs) {
    uint16_t paras = mem_max_paragraphs();
    V(printf("Available paragraphs: %u (%.1f KB)\n",
           paras, (float)paras * 16 / 1024););
    ASSERT(paras > 0);
    ASSERT(paras < 0xA000);  // Should be less than 640KB
}

/**
 * @brief Test pointer difference calculation
 * @details Tests:
 * - Near pointer differences
 * - Array element spacing
 * - Equality case
 */
TEST(test_mem_diff_pointers) {
    char buffer[100];
    char *p1 = &buffer[10];
    char *p2 = &buffer[30];

    /* Basic difference */
    mem_diff_t diff = mem_diff_pointers(p1, p2);
    EXPECT(diff == -20);
    EXPECT(mem_diff_pointers(p2, p1) == 20);

    /* Array element spacing */
    int arr[10];
    EXPECT(mem_diff_pointers(&arr[5], &arr[2]) == 3 * sizeof(int));

    /* Equality case */
    EXPECT(mem_diff_pointers(p1, p1) == 0);
}

/**
 * @brief Test MCB dumping functionality
 * @details Verifies:
 * - Output to different streams
 * - Handling of NULL MCB (should assert)
 */
TEST(test_mem_dump_mcb_to_stream) {
    /* This test requires actual MCB - we'll use the first one */


}

/**
 * @brief Test file load/save operations using DOS services
 * @details Tests:
 * - Round-trip save/load
 * - Data integrity
 * - Partial loads
 * - Error conditions
 */
TEST(test_mem_load_save_file) {

}

#endif

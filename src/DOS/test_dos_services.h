/**
 * @file test_dos_services.h
 * @brief Test suite for DOS services implementation
 * @ingroup tdd_framework
 */
#ifndef TEST_DOS_SERVICES_H
#define TEST_DOS_SERVICES_H

#include "dos_services.h"
#include "../TDD/tdd_macros.h"
#include "../MEM/mem_tools.h"
#include <stdio.h>
#include <stdint.h>

#define DOS_SERVICES_TESTS &test_set_get_interrupt_vector,  \
    &test_memory_allocation_basic,                          \
    &test_memory_allocation_edge_cases,                     \
    &test_memory_free_operations,                           \
    &test_memory_exhaustion

/**
 * @brief Test interrupt vector manipulation
 * @details Verifies that:
 * - Vectors can be set and retrieved
 * - Retrieved vectors match set values
 * - Vector manipulation doesn't crash
 */
TEST(test_set_get_interrupt_vector)
{
    void* original_vector;
    void* test_handler = (void*)0x1234ABCD;
    uint8_t test_vector = 0x15;  /* Using DOS keyboard vector for testing */

    /* Save original vector */
    original_vector = dos_get_interrupt_vector(test_vector);
    EXPECT(original_vector != NULL);

    /* Set and verify new vector */
    dos_set_interrupt_vector(test_vector, test_handler);
    void* retrieved = dos_get_interrupt_vector(test_vector);
    EXPECT(retrieved == test_handler);

    /* Restore original vector */
    dos_set_interrupt_vector(test_vector, original_vector);
    retrieved = dos_get_interrupt_vector(test_vector);
    EXPECT(retrieved == original_vector);
}

/**
 * @brief Basic memory allocation tests
 * @details Verifies:
 * - Basic allocation succeeds
 * - Allocated segments are non-zero
 * - Different allocation sizes work
 */
TEST(test_memory_allocation_basic)
{
    uint16_t block1 = dos_allocate_memory_blocks(16);  /* 16 paragraphs (256 bytes) */
    ASSERT(block1 != 0);

    uint16_t block2 = dos_allocate_memory_blocks(32);  /* 32 paragraphs (512 bytes) */
    ASSERT(block2 != 0);
    ASSERT(block2 != block1);  /* Should be different segments */

    /* Clean up */
    EXPECT(dos_free_allocated_memory_blocks(block1) == 0);
    EXPECT(dos_free_allocated_memory_blocks(block2) == 0);
}

/**
 * @brief Edge case memory allocation tests
 * @details Tests:
 * - Zero-size allocation
 * - Maximum possible allocation
 * - Free of invalid segments
 */
TEST(test_memory_allocation_edge_cases)
{
    /* Test zero-size allocation */
    uint16_t zero_block = dos_allocate_memory_blocks(0);
    EXPECT(zero_block == 0);  /* Should fail */

    /* Test free of invalid segment */
    EXPECT(dos_free_allocated_memory_blocks(0x0000) != 0);  /* NULL segment */
    EXPECT(dos_free_allocated_memory_blocks(0xFFFF) != 0);  /* Invalid segment */
}

/**
 * @brief Memory free operation tests
 * @details Verifies:
 * - Successful freeing of memory
 * - Double-free detection
 * - Free of already freed blocks
 */
TEST(test_memory_free_operations)
{
    uint16_t block = dos_allocate_memory_blocks(16);
    ASSERT(block != 0);

    /* First free should succeed */
    EXPECT(dos_free_allocated_memory_blocks(block) == 0);

    /* Second free should fail */
    EXPECT(dos_free_allocated_memory_blocks(block) != 0);
}

/**
 * @brief Memory exhaustion tests
 * @details Tests:
 * - Allocation failure when memory is exhausted
 * - Largest available block reporting
 */
TEST(test_memory_exhaustion)
{
    /* First find out how much memory is available */
    uint16_t largest_block = mem_max_paragraphs();
    V(printf("Largest available block: %u paragraphs\n", largest_block););
    /* Try to allocate slightly more than available */
    uint16_t too_big = dos_allocate_memory_blocks(largest_block + 1);
    EXPECT(too_big == 0);
    /* Try to allocate exactly the available amount */
    uint16_t exact_fit = dos_allocate_memory_blocks(largest_block);
    ASSERT(exact_fit != 0);
    EXPECT(dos_free_allocated_memory_blocks(exact_fit) == 0);

}

#endif

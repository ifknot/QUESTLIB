/**
 * @file test_mem_arena.h
 * @brief Test-driven development for memory arena
 * @defgroup arena_tests Memory Arena Tests
 * @{
 */
#ifndef TEST_MEM_H
#define TEST_MEM_H

#include <stdio.h>
#include <assert.h>
#include "mem_arena.h"
#include "../TDD/tdd_macros.h"
#include "mem_tools.h"

/// @brief Array of all test cases for the arena library
#define ARENA_TESTS &test_arena_creation, \
                    &test_basic_allocation, \
                    &test_allocation_limits, \
                    &test_deallocation, \
                    &test_zero_allocation, \
                    &test_null_arena_handling, \
                    &test_arena_dump

#define TEST_ARENA_SIZE (MEM_SIZE_1K)  // 1KB test arena

/* ----------------- Test Fixtures ----------------- */

static mem_arena_t* test_arena = NULL;

void setup() {
    test_arena = mem_arena_create(MEM_ARENA_POLICY_DOS, TEST_ARENA_SIZE);
}

void teardown() {
    if (test_arena) {
        mem_arena_delete(test_arena);
        test_arena = NULL;
    }
}

/* ----------------- Core Functionality Tests ----------------- */

TEST(test_arena_creation) {
    setup();

    // Verify arena creation
    ASSERT(test_arena != NULL);
    ASSERT(mem_arena_capacity(test_arena) == TEST_ARENA_SIZE);
    ASSERT(mem_arena_size(test_arena) == TEST_ARENA_SIZE);  // Initially all free
    ASSERT(mem_arena_used(test_arena) == 0);

    // Verify DOS-specific features
    if (mem_arena_policy(test_arena) == MEM_ARENA_POLICY_DOS) {
        char* mcb = mem_arena_dos_mcb(test_arena);
        ASSERT(mcb != 0);
        ASSERT(mem_diff_pointers(mcb, mem_arena_base_address(test_arena)) < 0);  // MCB should precede arena
        V(mem_dump_mcb_to_stream(stdout, mcb););
    }

    teardown();
}

TEST(test_basic_allocation) {
    setup();

    // Allocate small block
    char* block1 = (char*)mem_arena_alloc(test_arena, 64);
    ASSERT(block1 != NULL);
    ASSERT(block1 == (char*)mem_arena_base_address(test_arena));  // First allocation at start
    ASSERT(mem_arena_used(test_arena) == 64);
    ASSERT(mem_arena_size(test_arena) == TEST_ARENA_SIZE - 64);

    // Subsequent allocation
    int* block2 = (int*)mem_arena_alloc(test_arena, sizeof(int) * 16);
    ASSERT(block2 != NULL);
    ASSERT((void*)block2 == (void*)(block1 + 64));

    teardown();
}

TEST(test_allocation_limits) {
    setup();

    // Exhaust entire arena
    void* full_block = mem_arena_alloc(test_arena, TEST_ARENA_SIZE);
    ASSERT(full_block != NULL);
    ASSERT(mem_arena_size(test_arena) == 0);

    // Should reject further allocation
    void* should_fail = mem_arena_alloc(test_arena, 1);
    ASSERT(should_fail == NULL);

    teardown();
}

TEST(test_deallocation) {
    setup();

    // Allocate and deallocate
    char* block = (char*)mem_arena_alloc(test_arena, 256);
    mem_arena_dealloc(test_arena, 256);

    // Verify pointer reset
    ASSERT(mem_arena_free_address(test_arena) == mem_arena_base_address(test_arena));
    ASSERT(mem_arena_size(test_arena) == TEST_ARENA_SIZE);

    // Verify reusability
    char* new_block = (char*)mem_arena_alloc(test_arena, 256);
    ASSERT(new_block == block);  // Should reuse same space

    teardown();
}

/* ----------------- Edge Case Tests ----------------- */


TEST(test_zero_allocation) {
#ifdef NDEBUG
    setup();

    void* block = mem_arena_alloc(test_arena, 0);
    ASSERT(block == NULL);
    ASSERT(mem_arena_used(test_arena) == 0);

    teardown();
#endif
}

TEST(test_null_arena_handling) {
#ifdef NDEBUG
    // Should handle NULL arenas gracefully
    ASSERT(mem_arena_alloc(NULL, 10) == NULL);
    ASSERT(mem_arena_dealloc(NULL, 10) == NULL);
    ASSERT(mem_arena_delete(NULL) == 0);
    ASSERT(mem_arena_dos_mcb(NULL) == NULL);
#endif
}

/* ----------------- Main Test Runner ----------------- */

TEST(test_arena_dump) {
    setup();

    // Verify dump doesn't crash
    V(mem_arena_dump(stdout, test_arena););

    // Allocate some memory and dump again
    mem_arena_alloc(test_arena, 128);
    V(mem_arena_dump(stderr, test_arena););

    teardown();
}

#endif

/** @} */ // end of arena_tests group

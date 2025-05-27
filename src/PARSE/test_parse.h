#ifndef TEST_PARSE_H
#define TEST_PARSE_H
/**
 * @file test_mem_arena.h
 * @brief Test-driven development for memory arena
 * @defgroup arena_tests Memory Arena Tests
 * @{
 */
#include <stdio.h>
#include <assert.h>
#include "mem_arena.h"
#include "../TDD/tdd_macros.h"

/// @brief Array of all test cases for the arena library
#define PARSE_TESTS 
        test_arena_creation,                    /
        test_basic_allocation,                  /
        test_allocation_limits,                 /
        test_deallocation,                      /
        test_zero_allocation,                   /
        test_null_arena_handling,               /
        test_arena_dump                         /

#define TEST_ARENA_SIZE (MEM_SIZE_1K)  // 1KB test arena

/* ----------------- Test Fixtures ----------------- */

static mem_arena_t* test_arena = NULL;

void setup() {
    test_arena = mem_arena_new(MEM_ARENA_POLICY_DOS, TEST_ARENA_SIZE);
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
    if (test_arena->policy == MEM_ARENA_POLICY_DOS) {
        char* mcb = mem_arena_dos_mcb(test_arena);
        ASSERT(mcb != NULL);
        ASSERT(mcb < test_arena->start.ptr);  // MCB should precede arena
    }
    
    teardown();
}

TEST(test_basic_allocation) {
    setup();
    
    // Allocate small block
    char* block1 = mem_arena_alloc(test_arena, 64);
    ASSERT(block1 != NULL);
    ASSERT(block1 == test_arena->start.ptr);  // First allocation at start
    ASSERT(mem_arena_used(test_arena) == 64);
    ASSERT(mem_arena_size(test_arena) == TEST_ARENA_SIZE - 64);
    
    // Subsequent allocation
    int* block2 = mem_arena_alloc(test_arena, sizeof(int) * 16);
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
    char* block = mem_arena_alloc(test_arena, 256);
    mem_arena_dealloc(test_arena, 256);
    
    // Verify pointer reset
    ASSERT(test_arena->free == test_arena->start.ptr);
    ASSERT(mem_arena_size(test_arena) == TEST_ARENA_SIZE);
    
    // Verify reusability
    char* new_block = mem_arena_alloc(test_arena, 256);
    ASSERT(new_block == block);  // Should reuse same space
    
    teardown();
}

/* ----------------- Edge Case Tests ----------------- */

TEST(test_zero_allocation) {
    setup();
    
    void* block = mem_arena_alloc(test_arena, 0);
    ASSERT(block == NULL);
    ASSERT(mem_arena_used(test_arena) == 0);
    
    teardown();
}

TEST(test_null_arena_handling) {
    // Should handle NULL arenas gracefully
    ASSERT(mem_arena_alloc(NULL, 10) == NULL);
    ASSERT(mem_arena_dealloc(NULL, 10) == NULL);
    ASSERT(mem_arena_delete(NULL) == 0);
    ASSERT(mem_arena_dos_mcb(NULL) == NULL);
}

/* ----------------- Main Test Runner ----------------- */

TEST(test_arena_dump) {
    setup();
    
    // Verify dump doesn't crash
    mem_arena_dump(stdout, test_arena);
    
    // Allocate some memory and dump again
    mem_arena_alloc(test_arena, 128);
    mem_arena_dump(stderr, test_arena);
    
    teardown();
}

/** @} */ // end of arena_tests group

#endif



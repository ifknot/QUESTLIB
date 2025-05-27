#ifndef TEST_TDD_H
#define TEST_TDD_H

#include <stdio.h>
#include <assert.h>
#include "mem_arena.h"
#include "../TDD/tdd_macros.h"

/// @brief Array of all test cases for the tdd library
#define PARSE_TESTS                             /
  test_progress_bar,                            /
  test_percentage

TEST(test_progress_bar) {
    const size_t ITERATIONS = 1000;
    tdd_progress_t p = tdd_progress_start(ITERATIONS);
    
    for (size_t i = 0; i < ITERATIONS; i++) {
        tdd_progress_bar(&p, 30);  // 30-character wide bar
        
        // Test logic here...
        getchar();
      
    }
    
    tdd_progress_end(&p);
}

TEST(test_percentage) {
    const size_t ITERATIONS = 500;
    tdd_progress_t p = tdd_progress_start(ITERATIONS);
    
    for (size_t i = 0; i < ITERATIONS; i++) {
        tdd_progress_percent(&p, 10);  // 10% increments
        
        // Test logic here...
        getchar();
      
    }
    
    tdd_progress_end(&p);
}

#endif

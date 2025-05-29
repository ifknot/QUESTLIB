#ifndef TEST_TDD_H
#define TEST_TDD_H

#include <stdio.h>
#include <assert.h>
#include "tdd_progress.h"
#include "tdd_spinner.h"
#include "../TDD/tdd_macros.h"

/// @brief Array of all test cases for the tdd library
#define TDD_TESTS &test_progress_bar, &test_percentage, &test_spinner

TEST(test_progress_bar) {
    const size_t ITERATIONS = 1000;
    tdd_progress_t prg = tdd_progress_start(ITERATIONS, 0, 0, 30); // 30-character wide bar starting at zero and calculating steps
    for (size_t i = 0; i < ITERATIONS; i++) {
        tdd_progress_bar(&prg);  

        // Test logic here...

    }
    tdd_progress_end(&prg);
}

TEST(test_percentage) {
    const size_t ITERATIONS = 5000;
    tdd_progress_t prg = tdd_progress_start(ITERATIONS, 0, 10, 0); // starting at 0, 10% increments - width not relevant
    for (size_t i = 0; i < ITERATIONS; i++) {
        tdd_progress_percent(&prg);  

        // Test logic here...
    }
    tdd_progress_end(&prg);
}

TEST(test_spinner) {
    const size_t ITERATIONS = 5000;
    tdd_spinner_t spin = tdd_spinner_start(NULL); // Default spinner
    for (size_t i = 0; i < ITERATIONS; i++) {
        tdd_spinner_step(&spin);

        // Test logic here...
    }
    tdd_spinner_end(&spin);
}

#endif

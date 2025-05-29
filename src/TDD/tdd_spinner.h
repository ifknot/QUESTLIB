/**
 * @file tdd_spinner.h
 * @brief Test progress reporting utilities
 * @defgroup test_utils Test Utilities
 * @{
 */
#ifndef TDD_SPINNER_H
#define TDD_SPINNER_H

#include <stdlib.h>

typedef private_tdd_spinner_t tdd_spinner_t;

/**
 * @brief Initializes a spinner
 * @param frames Optional: Custom animation frames 
 * @return Initialized spinner
 *
 * @example
 * tdd_spinner_t spin = tdd_spinner_start(TDD_SPINNER_CP437_CHARS);
 * while (working) {
 *     tdd_spinner_step(&spin);
 *     // Work...
 * }
 * tdd_spinner_end(&spin);
 */
tdd_spinner_t tdd_spinner_start(const char* frames);

/**
 * @brief Updates spinner animation
 * @param s Spinner instance
 */
void tdd_spinner_step(tdd_spinner_t* s);

/**
 * @brief Cleans up spinner display
 * @param s Spinner instance
 */
void tdd_spinner_end(tdd_spinner_t* s);

#endif

/** @} */ // end of test_utils group

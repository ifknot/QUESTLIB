/**
 * @file tdd_spinner.h
 * @brief Test progress reporting utilities (now with spinner!)
 * @defgroup test_utils Test Utilities
 * @{
 */
#ifndef TDD_SPINNER_H
#define TDD_SPINNER_H

#include <stdio.h>
#include <time.h>
//#include <unistd.h> // For usleep() - not needed for IBM XT and similar

/* ----------------- Spinner Progress ----------------- */

typedef struct {
    size_t counter;
    const char* spinner_frames;
} tdd_spinner_t;

/**
 * @brief Initializes a spinner
 * @param frames Optional: Custom animation frames (default "|/-\\")
 * @return Initialized spinner
 * 
 * @example
 * tdd_spinner_t spin = tdd_spinner_start();
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
 * @param delay_ms Delay between frames (0 for no delay)
 */
void tdd_spinner_step(tdd_spinner_t* s, int delay_ms);

/**
 * @brief Cleans up spinner display
 * @param s Spinner instance
 */
static inline void tdd_spinner_end(tdd_spinner_t* s);

#endif

/** @} */ // end of test_utils group

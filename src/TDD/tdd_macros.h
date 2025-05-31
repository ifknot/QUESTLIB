/**
 * @file tdd_macros.h
 * @brief Minimalist Test-Driven Development (TDD) framework
 * @author (modified version of) Axel Lindeberg aka u/SuperSmurfen
 * @see Original implementation: https://github.com/AxlLind/libwebb/blob/main/tests/libtest.h
 * @see Example usage: https://github.com/AxlLind/libwebb/blob/main/tests/test_http.c
 *
 * @defgroup tdd_framework Test Framework
 * @{
 */

#ifndef TDD_MACROS_H
#define TDD_MACROS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tdd_progress.h"

/**
 * @brief Conditional verbose output macro
 * @param expr Expression to execute only in debug mode
 */
#ifndef NDEBUG
#define V(expr) do { { expr } } while(0)
#else
#define V(expr)
#endif

/**
 * @brief Extracts filename from full path
 */
#define FILENAME (strrchr("/" __FILE__, '/') + 1)

/**
 * @brief Internal assertion handler
 * @param expr Expression to evaluate
 * @param halt Whether to stop test execution on failure
 * @private
 */
#define ASSERT_HALT(expr, halt)                                         \
    do {                                                                \
        if (!(expr)) {                                                  \
            printf("\n%s:%d - expected: %s\n", FILENAME, __LINE__, #expr);\
            *pass = false;                                              \
            if (halt)                                                   \
                return;                                                 \
        }                                                               \
    } while (0)

/**
 * @brief Non-fatal test expectation
 * @param expr Expression to test (continues execution if false)
 */
#define EXPECT(expr) ASSERT_HALT(expr, false)

/**
 * @brief Fatal test assertion
 * @param expr Expression to test (stops test if false)
 */
#define ASSERT(expr, ...) ASSERT_HALT(expr, true)

/**
 * @brief Test case structure
 */
typedef struct {
    void (*fn)(bool*);   /**< Test function pointer */
    char *name;          /**< Test name */
} test_t;

/**
 * @brief Declares a test case
 * @param name Test case name
 *
 * @details Creates:
 *          1. Test function prototype
 *          2. Test case structure
 *          3. Test function implementation
 */
#define TEST(name)                                  \
    static void name##_fn(bool*);                   \
    static const test_t name = {name##_fn, #name};  \
    static void name##_fn(bool* pass)

/**
 * @brief Executes a test suite
 * @param ... Variable list of test cases
 * @return Number of failed tests
 *
 * @details Handles:
 *          - Test execution
 *          - Result reporting
 *          - Failure counting
 *          - Verbosity control
 */
#ifndef NDEBUG
#define RUN_TESTS(...)                                                          \
    int run_tests(void) {                                                       \
        const test_t* tests[] = {__VA_ARGS__};                                  \
        int failures = 0;                                                       \
        for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {         \
            bool passed = true;                                                 \
            tests[i]->fn(&passed);                                              \
            printf("\n%s: %s\n", passed ? "PASS" : "FAIL", tests[i]->name); \
            if (!passed)                                                        \
                failures++;                                                     \
        }                                                                       \
        V(printf("Failures = %i", failures););                                  \
        return failures;                                                        \
    }
#else
#define RUN_TESTS(...)                                                          \
    int run_tests(void) {                                                       \
        printf("Testing\n");                                                    \
        const test_t* tests[] = {__VA_ARGS__};                                  \
        int iterations = sizeof(tests) / sizeof(tests[0]);                      \
        int failures = 0;                                                       \
        for (size_t i = 0; i < iterations; i++) {                               \
            bool passed = true;                                                 \
            tests[i]->fn(&passed);                                              \
            printf("%c",'.');                                                   \
            fflush(stdout);                                                     \
            if (!passed)                                                        \
                failures++;                                                     \
        }                                                                       \
        printf("\n%s",(failures) ? "Fail :(" : "Pass :)");                      \
        return failures;                                                        \
    }
#endif
#endif

/** @} */ // end of tdd_framework group

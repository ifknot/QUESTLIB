/**
* Thank you Axel Lindeberg u/SuperSmurfen
* https://github.com/AxlLind/libwebb/blob/main/tests/libtest.h
* See usage here for example:
* https://github.com/AxlLind/libwebb/blob/main/tests/test_http.c
*/
#ifndef TDD_MACROS_H
#define TDD_MACROS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FILENAME (strrchr("/" __FILE__, '/') + 1)

#define ASSERT_HALT(expr, halt)                                     \
    do {                                                                \
        if (!(expr)) {                                                  \
            printf("%s:%d - expected: %s\n", FILENAME, __LINE__, #expr);\
            *pass = false;                                              \
            if (halt)                                                   \
                return;                                                 \
        }                                                               \
    } while (0)

// continues to the next test regardless of the result
#define EXPECT(expr) ASSERT_HALT(expr, false)
// returns from the test function if the expression is false
#define ASSERT(expr) ASSERT_HALT(expr, true)

typedef struct {
    void (*fn)(bool*);
    char *name;
} test_t;

#define TEST(name)                                  \
    static void name##_fn(bool*);                   \
    static const test_t name = {name##_fn, #name};  \
    static void name##_fn(bool* pass)

#define RUN_TESTS(...)                                                          \
    int run_tests(void) {                                                       \
        const test_t* tests[] = {__VA_ARGS__};                                  \
        int failures = 0;                                                       \
        for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {         \
            bool passed = true;                                                 \
            tests[i]->fn(&passed);                                              \
            printf("%s: %s\n", passed ? "PASSED" : "FAILED", tests[i]->name);   \
            if (!passed)                                                        \
                failures++;                                                     \
        }                                                                       \
        return failures ? EXIT_FAILURE : EXIT_SUCCESS;                          \
    }

#endif

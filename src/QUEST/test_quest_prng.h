/**
 * @file test_prng.h
 * @brief All-in-one PRNG test harness with Monte Carlo and visual tests
 *
 * @usage
 * #define TEST_PRNGRNG_IMPLEMENTATION
 * #include "test_prng.h"
 *
 * int main() {
 *     mem_arena_t arena = mem_arena_create(...);
 *     run_prng_test_suite(&arena);
 *     mem_arena_delete(&arena);
 * }
 */

#ifndef TEST_PRNG_H
#define TEST_PRNG_H

#include <math.h>
#include <stdio.h>

#include "../TDD/tdd_macros.h"

#include "quest_prng.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define QUEST_PRNG_TESTS \
                    &test_prng_basic_functionality, \
                    &test_monte_carlo_pi,           \
                    &test_distribution,             \
                    //&test_ascii_noise

TEST(test_prng_basic_functionality) {
    quest_prng_ctx_t rng;
    quest_prng_seed(&rng, 0);

    // Verify zero-seed correction
    uint32_t first = quest_prng_generate(&rng);
    EXPECT_NEQ(first, 0);

    // Verify sequence reproducibility
    quest_prng_seed(&rng, 42);
    uint32_t seq[3];
   for(int i = 0; i < 3; ++i) {
        seq[i] = quest_prng_generate(&rng);
    };
    quest_prng_seed(&rng, 42);
    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(seq[i], quest_prng_generate(&rng));
    }
}

TEST(test_monte_carlo_pi) {
    quest_prng_ctx_t rng;
    quest_prng_seed(&rng, quest_prng_seed_from_time());

    const int samples = 500000;
    int inside = 0;

    for (int i = 0; i < samples; i++) {
        double x = quest_prng_generate(&rng) / (double)UINT32_MAX;
        double y = quest_prng_generate(&rng) / (double)UINT32_MAX;
        inside += (x*x + y*y) <= 1.0;
    }

    double pi_est = 4.0 * inside / samples;
    printf("\n PI Estimation: %.6f (Error: %.4f%%)\n", pi_est, fabs(pi_est - M_PI)/M_PI*100);
    //EXPECT_IN_RANGE(pi_est, 3.12, 3.16);
}

TEST(test_ascii_noise) {
    quest_prng_ctx_t rng;
    quest_prng_seed(&rng, 0xCAFEBABE);

    printf("\nASCII Noise:\n");
    const char* shades = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 50; x++) {
            char c = shades[quest_prng_range(&rng, 0, strlen(shades)-1)];
            putchar(c);
        }
        putchar('\n');
    }
    ASSERT(1); // Visual test always passes
}

TEST(test_distribution) {
    quest_prng_ctx_t rng;
    quest_prng_seed(&rng, time(NULL));

    int buckets[20] = {0};
    const int samples = 100000;

    for (int i = 0; i < samples; i++) {
        buckets[quest_prng_generate(&rng) % 20]++;
    }

    printf("\nDistribution:\n");
    for (int i = 0; i < 2; i++) {
        printf("%2d: %5d %s\n", i, buckets[i],
               buckets[i] > samples/25 ? ":)" : ":(");
        EXPECT_GT(buckets[i], 0);
    }
}

#endif

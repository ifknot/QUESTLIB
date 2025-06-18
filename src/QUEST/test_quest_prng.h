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
#include <stdint.h>
#include <stdio.h>

#include "../TDD/tdd_macros.h"
#include "../TDD/tdd_graphs.h"

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
    const tdd_size_t samples = 10000;//0;
    tdd_size_t inside_circle = 0;
    tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 40);
/*
    // Initialize random seed
    srand(time(NULL));
    for (tdd_size_t i = 0; i < samples; i++) {
        tdd_progress_bar(&prg);
        ON_ESCAPE_BREAK
        // Generate random point in [0,1) x [0,1)
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        // Check if point is inside unit circle
        if (x*x + y*y <= 1.0) {
            inside_circle++;
        }
    }
    printf("/ninside circle %lu, samples %lu", inside_circle, samples);
    // π ≈ 4 * (points inside circle) / (total points)
    double pi_est = 4.0 * (double)inside_circle / (double)samples;
*/
    quest_prng_ctx_t rng;
    quest_prng_seed(&rng, quest_prng_seed_from_time());

    for (tdd_size_t i = 0; i < samples; i++) {
        double x = (double)quest_prng_generate(&rng) / (double)UINT32_MAX;
        double y = (double)quest_prng_generate(&rng) / (double)UINT32_MAX;
        inside_circle += (x*x + y*y) <= 1.0;
        tdd_progress_bar(&prg);
        ON_ESCAPE(break;);
    }

    double pi_est = 4.0 * (double)inside_circle / (double)samples;
    printf("\n PI Estimation: %.6f (Error: %.4f%%)\n", pi_est, fabs(pi_est - M_PI)/M_PI*100);
    EXPECT_IN_RANGE(pi_est, 3.12, 3.16);
}

TEST(test_ascii_noise) {
    quest_prng_seed(&quest_global_ctx, 0xCAFEBABE);

    printf("\nASCII Noise:\n");
    const char* shades = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 50; x++) {
            char c = shades[quest_prng_range(&quest_global_ctx, 0, strlen(shades)-1)];
            putchar(c);
        }
        putchar('\n');
    }
    ASSERT(1); // Visual test always passes
}

TEST(test_distribution) {
    #define NBUCKETS 20
    quest_prng_ctx_t rng;
    quest_prng_seed(&rng, time(NULL));

    tdd_size_t buckets[NBUCKETS] = {0};
    const tdd_size_t samples = 10000;//0;
    tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 40);
    tdd_size_t i;

    for (i = 0; i < samples; i++) {
        buckets[quest_prng_generate(&rng) % 20]++;
        tdd_progress_bar(&prg);
        ON_ESCAPE(break;);
    }
    tdd_histogram(buckets, NBUCKETS, 50); // 50 = max bar width
    PRESS_ENTER;
}

#endif

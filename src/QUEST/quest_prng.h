/**
 * @file quest_prng.h
 * @brief XORSHIFT32 PRNG with Global Context
 *
 * @section setup_global Setting Up Global Context
 * @code
 * // In ONE source file (e.g., main.c):
 * #include "quest_prng.h"
 * quest_prng_ctx_t quest_global_ctx;  // Define global instance
 *
 * // At program start:
 * int main() {
 *     quest_prng_seed(&quest_global_ctx, quest_prng_seed_from_time());
 *     // ... rest of initialization
 * }
 * @endcode
 *
 * @section using_global Using Global Context
 * @code
 * // In any file that includes quest_prng.h:
 * void spawn_enemy() {
 *     if (quest_prng_roll(&quest_global_ctx, 30)) { // 30% chance
 *         // Critical spawn logic
 *     }
 *
 *     uint32_t damage = quest_prng_range(&quest_global_ctx, 5, 15);
 * }
 * @endcode
 *
 */
#ifndef QUEST_PRNGRNG_H
#define QUEST_PRNGRNG_H

#include <stdint.h>
#include <time.h>

/**
 * @brief PRNG context type
 * @example
 * // File-scoped static context:
 * static quest_prng_ctx_t my_rng;
 */
typedef uint32_t quest_prng_ctx_t;

/**
 * @brief Global PRNG context
 * @warning Must be defined in ONE .c file
 * @example
 * // In main.c:
 * quest_prng_ctx_t quest_global_ctx;
 */
extern quest_prng_ctx_t quest_global_ctx;

// Core API --------------------------------------------------------

/**
 * @brief Seeds the PRNG
 * @example
 * // Seed with fixed value for reproducibility:
 * quest_prng_seed(&ctx, 0x12345678);
 */
void quest_prng_seed(quest_prng_ctx_t* ctx, uint32_t seed);

/**
 * @brief Generates random number
 * @example
 * // Shuffle algorithm using global context:
 * for (int i = 0; i < n; i++) {
 *     uint32_t j = quest_prng_generate(&quest_global_ctx) % n;
 *     swap(items[i], items[j]);
 * }
 */
uint32_t quest_prng_generate(quest_prng_ctx_t* ctx);

// Helpers ---------------------------------------------------------

/**
 * @brief Time-based seed generation
 * @example
 * // Typical initialization:
 * quest_prng_seed(&ctx, quest_prng_seed_from_time());
 */
static inline uint32_t quest_prng_seed_from_time(void) {
    return (uint32_t)(time(NULL) ^ (clock() << 16));
}

/**
 * @brief Percentage chance check
 * @example
 * // Loot drop with 15% chance:
 * if (quest_prng_roll(&quest_global_ctx, 15)) {
 *     spawn_rare_item();
 * }
 */
static inline int quest_prng_roll(quest_prng_ctx_t* ctx, int percentage) {
    return (quest_prng_generate(ctx) % 100) < percentage;
}

/**
 * @brief Random value in range [min,max]
 * @example
 * // Random enemy count between 3-5:
 * uint32_t enemies = quest_prng_range(&quest_global_ctx, 3, 5);
 */
static inline uint32_t quest_prng_range(quest_prng_ctx_t* ctx, uint32_t min, uint32_t max) {
    return min + (quest_prng_generate(ctx) % (max - min + 1));
}

#endif

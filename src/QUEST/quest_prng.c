#include "quest_prng.h"

void quest_prng_seed(quest_prng_ctx_t* ctx, uint32_t seed) {
    *ctx = seed ? seed : 0xBAD5EED;
    if (*ctx == 0) *ctx = 1; // XORSHIFT32 must not be zero
}

uint32_t quest_prng_generate(quest_prng_ctx_t* ctx) {
    uint32_t x = *ctx;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return (*ctx = x);
}

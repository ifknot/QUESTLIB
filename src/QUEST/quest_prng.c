#include "quest_prng.h"

#include <assert.h>

void quest_prng_seed(quest_prng_ctx_t* ctx, uint32_t seed) {
   assert(ctx && "NULL context!");
   *ctx = seed ? seed : 0xBAD5EED;
}

uint32_t quest_prng_generate(quest_prng_ctx_t* ctx) {
    uint32_t x = *ctx;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return (*ctx = x);
}

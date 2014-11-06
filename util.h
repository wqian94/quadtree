/**
Some utility functions
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

/**
 * MarsagliaXORV
 *
 * Provided by @amatveev
 */
static inline uint32_t MarsagliaXORV(uint32_t x) {
    if (x == 0)
        x = 1;
    x ^= x << 6;
    x ^= ((unsigned)x) >> 21;
    x ^= x << 7 ;
    return x;  // use either x or x & 0x7FFFFFFF
}

/**
 * MarsagliaXOR
 *
 * Provided by @amatveev
 */
static inline uint32_t MarsagliaXOR(uint32_t* seed) {
    unsigned x = MarsagliaXORV(*seed);
    *seed = x;
    return x & 0x7FFFFFFF;
}

static uint32_t Marsaglia_seed;

static uint32_t Marsaglia_rand() {
    return (unsigned)MarsagliaXOR(&Marsaglia_seed);
}

static void Marsaglia_srand(uint32_t nseed) {
    Marsaglia_seed = nseed;
}

#endif

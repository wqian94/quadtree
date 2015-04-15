/**
Some utility functions
*/

#ifndef UTIL_H
#define UTIL_H

#include <pthread.h>
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

/**
 * Marsaglia_random
 *
 * Returns a random value between 0 (inclusive) and 1 (exclusive).
 */
static double Marsaglia_random() {
    const uint32_t denom = 1e8;
    const uint32_t num = Marsaglia_rand();
    return (num % denom) / (double)denom;
}

/**
 * Marsaliga_rands
 *
 * Generates a pseudorandom integer from the given seed. The seed is mutated in-place.
 *
 * seed - the seed to use
 *
 * Returns a random integer.
 */
static uint32_t Marsaglia_rands(uint32_t *seed) {
    return (unsigned)MarsagliaXOR(seed);
}

/**
 * Marsaliga_randoms
 *
 * Generates a pseudorandom value from the given seed. The seed is mutated in-place.
 *
 * seed - the seed to use
 *
 * Returns a random value between [0, 1).
 */
static double Marsaglia_randoms(uint32_t *seed) {
    const uint32_t denom = 1e8;
    const uint32_t num = Marsaglia_rands(seed);
    return (num % denom) / (double)denom;
}

static void Marsaglia_srand(uint32_t nseed) {
    Marsaglia_seed = nseed;
}

static volatile pthread_mutexattr_t lock_attr, *lock_attr_ptr = NULL;

static void pthread_mutex_attr_init() {
    pthread_mutexattr_init((pthread_mutexattr_t*)&lock_attr);
    pthread_mutexattr_settype((pthread_mutexattr_t*)&lock_attr, PTHREAD_MUTEX_ERRORCHECK);
    lock_attr_ptr = &lock_attr;
}

static pthread_mutexattr_t* pthread_mutex_attr() {
    return (pthread_mutexattr_t*)lock_attr_ptr;
}

static void pthread_mutex_attr_destroy() {
    lock_attr_ptr = NULL;
    pthread_mutexattr_destroy((pthread_mutexattr_t*)&lock_attr);
}

#endif

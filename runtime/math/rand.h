#ifndef CSCRATCH_RAND_H
#define CSCRATCH_RAND_H

#include <cstdint>
#include <cmath>
#include <ctime>

#define M_RAND_MAX 2147483647


static uint64_t g_seed = ((uint64_t)time(NULL) | 0x7FEDCBA9) << 32;

// Used to seed the generator.           
static inline void seedFastRand(uint64_t seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer in range [0, 2147483647]
static inline uint64_t fastRand() {
    g_seed = 214013 * g_seed + 2531021;
    return (g_seed >> 32) & 0x7FFFFFFF;
}

// Compute a pseudorandom double in range [0, 1]
static inline double rand01() {
    return (double)fastRand() / M_RAND_MAX;
}

// Returns a random real number in range [min, max]
// BUT if min AND max are both integers, then it will return only integers from the range
static inline double randInRange(double min, double max) {
    const double res = (max - min) * rand01() + min;
    return min == (int64_t)min && max == (int64_t)max // if both ints
        ? (int64_t)res
        : res;
}

#endif


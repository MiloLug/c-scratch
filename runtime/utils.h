#ifndef CSCRATCH_UTILS_H
#define CSCRATCH_UTILS_H

#include <ctime>


extern clock_t start_time;


double __getTime();
#define timer (__getTime())
void resetTimer();

#define SWAP(a, b) ({auto __tmp = (b); b = a; a = __tmp;})

#if defined(__clang__)
    #define restrict__ __restrict
#elif defined(__GNUC__) || defined(__GNUG__)
    #define restrict__ __restrict__
#elif defined(_MSC_VER)
    #define restrict__ __restrict
#else
    #define restrict__ /* no-op */
#endif


#endif

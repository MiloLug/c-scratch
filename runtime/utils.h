#ifndef CSCRATCH_UTILS_H
#define CSCRATCH_UTILS_H

#include <ctime>


extern clock_t start_time;


double __getTime();
#define timer (__getTime())
void resetTimer();

#define SWAP(a, b) ({auto __tmp = (b); b = a; a = __tmp;})

#endif

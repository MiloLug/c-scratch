#include "utils.h"


clock_t start_time = clock();


double __getTime() {
    return double(clock() - start_time) / CLOCKS_PER_SEC;
}

void resetTimer() {
    start_time = clock();
}

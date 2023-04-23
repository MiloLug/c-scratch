#ifndef CSCRATCH_FAST_MATH_H
#define CSCRATCH_FAST_MATH_H

#include <cstdint>


void printSinTable(int64_t tableSize);

double degSin(double degAngle);
double degCos(double degAngle);

#define radSin(angle) degSin((angle) / M_RAD)
#define radCos(angle) degCos((angle) / M_RAD)


#endif

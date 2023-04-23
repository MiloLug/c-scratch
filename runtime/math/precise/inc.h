#ifndef CSCRATCH_PRECISE_MATH_H
#define CSCRATCH_PRECISE_MATH_H

#include <cmath>
#include <cstdint>
#include <cfloat>


int __rem_pio2_large(double *x, double *y, int e0, int nx, int prec);
int __rem_pio2(double x, double *y);
double __sin(double x, double y, int iy);
double __cos(double x, double y);

double radSin(double radAngle);
double radCos(double radAngle);

#define degSin(angle) radSin((angle) * M_RAD)
#define degCos(angle) radCos((angle) * M_RAD)


#endif

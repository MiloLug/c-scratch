#ifndef CSCRATCH_MATH_H
#define CSCRATCH_MATH_H

#include "common.h"
#include "config.h"
#include "rand.h"
#include "sign.h"
#include "number_type.h"
#include "exp.h"

#include <cmath>

#ifdef ENABLE_FAST_MATH
    #include "fast/inc.h"
#else
    #include "precise/inc.h"
#endif

#define radTan(radAngle) tan(radAngle)
#define degTan(angle) tan((angle)*M_RAD)

#define radAsin(x) (asin(x))
#define degAsin(x) (asin(x) / M_RAD)

#define radAcos(x) (acos(x))
#define degAcos(x) (acos(x) / M_RAD)

#define radAtan(x) (atan(x))
#define degAtan(x) (atan(x) / M_RAD)

#endif

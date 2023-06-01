#ifndef CSCRATCH_MATH_H
#define CSCRATCH_MATH_H

#include "common.h"
#include "config.h"
#include "rand.h"
#include "sign.h"
#include "number_type.h"

#include <cmath>

#ifdef ENABLE_FAST_MATH
    #include "fast/inc.h"
#else
    #include "precise/inc.h"
#endif

#define radTan(radAngle) tan(radAngle)

#define degTan(angle) tan((angle)*M_RAD)

#endif

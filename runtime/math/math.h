#ifndef CSCRATCH_MATH_H
#define CSCRATCH_MATH_H

#include "common.h"
#include "config.h"
#include "rand.h"
#include "sign.h"

#include <cmath>

#ifdef ENABLE_FAST_MATH
    #include "fast/inc.h"
#else
    #include "precise/inc.h"
#endif

#endif

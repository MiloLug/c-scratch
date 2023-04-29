#ifndef CSCRATCH_MATH_H
#define CSCRATCH_MATH_H

#include "config.h"

#include "math/common.h"
#include "math/sign.h"
#include "math/rand.h"

#ifdef ENABLE_FAST_MATH
    #include "math/fast/inc.h"
#else
    #include "math/precise/inc.h"
#endif

#endif

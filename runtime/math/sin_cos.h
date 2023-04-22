#ifndef CSCRATCH_TRIGONOMETRY_H
#define CSCRATCH_TRIGONOMETRY_H

#include "config.h"

#ifdef  __cplusplus
    extern "C" {
#endif

    #ifdef MATH_USE_FAST_TRIGONOMETRY

        #include <cstdint>

        void printSinTable(int64_t tableSize);
        double mSin(double angle);
        double mCos(double angle);
    
    #else

        #include <cmath>
        #include "const.h"

        #define mSin(angle) sin((angle) * M_RAD)
        #define mCos(angle) cos((angle) * M_RAD)

    #endif

#ifdef  __cplusplus
    }
#endif

#endif

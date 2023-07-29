#ifndef CSCRATCH_PEN_LINES_H
#define CSCRATCH_PEN_LINES_H

#include <cstdint>

#include "_pixel.h"
#include "runtime/math/common.h"

#if defined(__AVX2__) || defined(__AVX__)
    #include <emmintrin.h>
    #include <immintrin.h>
#elif defined(__ARM_NEON__)
    #include <arm_neon.h>
#endif

#include <math.h>


namespace Pen {
    void drawHorizontalLine1(int64_t x1, int64_t x2, int64_t y, uint32_t color);
    void drawLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint64_t d, uint32_t color);
}


#endif

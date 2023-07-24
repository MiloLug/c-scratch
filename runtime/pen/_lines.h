#ifndef CSCRATCH_PEN_LINES_H
#define CSCRATCH_PEN_LINES_H

#include <cstdint>

#include "_pixel.h"
#include "runtime/math/common.h"

#ifdef __x86_64__
    #include <emmintrin.h>
    #include <immintrin.h>
#endif

#include <math.h>


namespace Pen {
    void drawHorizontalLine1(int64_t x1, int64_t x2, int64_t y, uint32_t color);
    void drawLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint64_t d, uint32_t color);
}


#endif

#ifndef CSCRATCH_PEN_FIG_H
#define CSCRATCH_PEN_FIG_H

#include <cstdint>
#include "_pixel.h"


namespace Pen {
    static inline void drawSquare2(int32_t x, int32_t y, uint32_t color) {
        drawPixel(x, y, color);
        drawPixel(x + 1, y, color);
        drawPixel(x + 1, y + 1, color);
        drawPixel(x, y + 1, color);
    }

    void drawCircle(int32_t cX, int32_t cY, int32_t r, uint32_t color);
}


#endif

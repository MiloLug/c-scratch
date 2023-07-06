#ifndef CSCRATCH_PEN_PIXEL_H
#define CSCRATCH_PEN_PIXEL_H

#include "config.h"
#include "runtime/include_sdl.h"
#include "runtime/mutex.h"

#include <cstdint>
#include <cstring>


#define __Pen_drawPixelOnPointer_unsafe(__bgPointer, __color, __srcAlpha)                          \
    if ((__srcAlpha) == 0xFF) {                                                                    \
        *(__bgPointer) = __color;                                                                  \
    } else {                                                                                       \
        const uint32_t bgColor = *(__bgPointer);                                                   \
        uint32_t bgA = bgColor & 0xFF;                                                             \
        uint32_t resA = __srcAlpha + (bgA * (255 - __srcAlpha) >> 8);                              \
                                                                                                   \
        uint32_t rb = bgColor >> 8 & 0x00FF00FF;                                                   \
        uint32_t g = bgColor & 0x00FF0000;                                                         \
        rb += (((__color) >> 8 & 0x00FF00FF) - rb) * __srcAlpha >> 8;                              \
        g += (((__color)&0x00FF0000) - g) * __srcAlpha >> 8;                                       \
                                                                                                   \
        *(__bgPointer) = (rb & 0x00FF00FF) << 8 | (g & 0x00FF0000) | resA;                         \
    }


namespace Pen {
    static constexpr int64_t canvasWidth = WINDOW_WIDTH, canvasHeight = WINDOW_HEIGHT;
    static constexpr uint64_t canvasSize = canvasWidth * canvasHeight;

    extern SDL_Texture * texture;
    extern volatile bool hasChanges;
    extern uint32_t pixelBuffer[canvasSize];
    extern Mutex pixels;

    static inline void eraseAll() { memset((void *)pixelBuffer, 0x00, canvasSize << 2); }

    static inline void drawPixel(int64_t x, int64_t y, uint64_t color) {
        if (x < 0 || x >= canvasWidth || y < 0 || y >= canvasHeight) return;

        const uint32_t srcA = color & 0xFF;
        const uint64_t pos = WINDOW_WIDTH * y + x;

        if (srcA == 0xFF) {
            pixelBuffer[pos] = color;
        } else {
            const uint32_t bgColor = pixelBuffer[pos];
            uint32_t resA = srcA + ((bgColor & 0xFF) * (255 - srcA) >> 8);

            uint32_t rb = bgColor >> 8 & 0x00FF00FF;
            uint32_t g = bgColor & 0x00FF0000;
            rb += ((color >> 8 & 0x00FF00FF) - rb) * srcA >> 8;
            g += ((color & 0x00FF0000) - g) * srcA >> 8;

            pixelBuffer[pos] = (rb & 0x00FF00FF) << 8 | (g & 0x00FF0000) | resA;
        }
    }
}


#endif

#ifndef CSCRATCH_PEN_PIXEL_H
#define CSCRATCH_PEN_PIXEL_H

#include <cstdint>
#include "config.h"
#include "runtime/include_sdl.h"
#include "runtime/mutex.h"


#if !defined ENABLE_TURBO && !defined ENABLE_UNSAFE_NO_LOCKS
    /*
    * Blocks the canvas for rendering while executing the code.
    *
    * After execution, sets `hasChanges` variable to tell the renderer to update the image.
    * Returns 1
    */
    #define Pen_safe(code) Pen::pixels.take(); code; Pen::hasChanges = true; Pen::pixels.release()
#else
    /*
    * Should block the canvas for rendering while executing the code
    * BUT you have `ENABLE_TURBO` or `ENABLE_UNSAFE_NO_LOCKS`, so it doesn't
    *
    * After execution, sets `hasChanges` variable to tell the renderer to update the image.
    * Returns 1
    */
    #define Pen_safe(code) code; Pen::hasChanges = true
#endif


namespace Pen {
    static constexpr const int64_t
        canvasWidth = WINDOW_WIDTH,
        canvasHeight = WINDOW_HEIGHT;
    static constexpr const uint64_t canvasSize = canvasWidth * canvasHeight;

    extern SDL_Texture * texture;
    extern volatile bool hasChanges;
    extern uint32_t volatile pixelBuffer[canvasSize];
    extern Mutex pixels;

    static inline void eraseAll() {
        memset((void *)pixelBuffer, 0xFF, canvasSize << 2);
    }

    static inline void drawPixel(int64_t x, int64_t y, uint64_t color) {
        if (x < 0 || x >= canvasWidth || y < 0 || y >= canvasHeight) return;

        const uint8_t alpha = (color >> 24);
        const uint64_t pos = WINDOW_WIDTH * y + x;

        if (alpha == 0xFF) {
            pixelBuffer[pos] = color;
        } else {
            const uint32_t bgColor = pixelBuffer[pos];

            uint32_t rb = bgColor & 0x00FF00FF;
            uint32_t g = bgColor & 0x0000FF00;
            rb += ((color & 0xFF00FF) - rb) * alpha >> 8;
            g += ((color & 0x00FF00) - g) * alpha >> 8;

            pixelBuffer[pos] = (rb & 0xFF00FF) | (g & 0x00FF00);
        }
    }
}


#endif

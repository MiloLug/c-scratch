#ifndef CSCRATCH_PEN_H
#define CSCRATCH_PEN_H

#include <cstdint>
#include <queue>
#include <utility>
#include <atomic>

#include "sdl.h"
#include "mutex.h"

#define RGBA(r, g, b, a)

namespace Pen {
    class Initializer {
    public:
        Initializer(ScratchSDLWindow &window);
        ~Initializer();
    private:
        static volatile bool initialized;
    };

    // color = 4 values x 8bit = 32bit
    // x = 1 value x 16bit
    // y = 1 value x 16bit
    // pixel = x + y + color = 64bit

    // typedef ThreadSafeQueue<std::pair<uint64_t, uint32_t>> changes_q;  // (index, pixel)

    static constexpr const uint32_t
        canvasWidth = WINDOW_WIDTH,
        canvasHeight = WINDOW_HEIGHT;
    static constexpr const uint64_t canvasSize = canvasWidth * canvasHeight;

    extern volatile SDL_Texture * texture;
    extern volatile bool hasChanges;
    extern volatile uint32_t pixelBuffer[canvasSize];
    extern Mutex pixels;
    
    // extern changes_q changeBuffer;
    // extern std::atomic<changes_q *> changes;

    // changes_q * takeChangesQ();
    // void releaseChangesQ(changes_q * changesQueue);

    void eraseAll();
    void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t thickness, uint32_t color);
    // void setPixel(int16_t x, int16_t y, uint32_t color);
    
};


#endif

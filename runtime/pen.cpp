#include "pen.h"

#include "math.h"

namespace Pen
{
    Initializer::Initializer(ScratchSDLWindow &window) {
        if (!initialized) {
            initialized = true;
            texture = SDL_CreateTexture(
                window.renderer,
                SDL_PIXELFORMAT_ARGB8888,
                SDL_TEXTUREACCESS_STREAMING,
                WINDOW_WIDTH,
                WINDOW_HEIGHT
            );

            memset((void *)pixelBuffer, 0xFF, canvasSize << 2);
        }
    }
    Initializer::~Initializer() {
        if (texture)
            SDL_DestroyTexture((SDL_Texture *)texture);
        texture = NULL;
    }
    volatile bool Initializer::initialized = false;

    // 1 pixel = 4 values x 8bit = 32bit

    volatile SDL_Texture *texture = NULL;
    volatile bool hasChanges = true;
    volatile uint32_t pixelBuffer[canvasSize];
    Mutex pixels;

    void eraseAll() {}

    static inline void drawPixel(int32_t x, int32_t y, uint32_t color) {
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

    static inline void drawLine1(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
        bool yLonger = false;
        int32_t shortLen = y2 - y1;
        int32_t longLen = x2 - x1;
        if (abs(shortLen) > abs(longLen)) {
            int32_t swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int32_t decInc;
        if (longLen == 0)
            decInc = 0;
        else
            decInc = (shortLen << 16) / longLen;

        if (yLonger) {
            if (longLen > 0)
            {
                longLen += y1;
                for (int32_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawPixel(j >> 16, y1, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int32_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawPixel(j >> 16, y1, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int32_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1)
            {
                drawPixel(x1, j >> 16, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int32_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawPixel(x1, j >> 16, color);
            j -= decInc;
        }
    }

    static inline void drawCircle(int32_t cX, int32_t cY, int32_t r, uint32_t color) {
        const int64_t rr = r * r;

        for (int32_t x = -r; x < r ; x++)
        {
            int32_t height = roundDToI32(sqrt(rr - x * x));

            for (int32_t y = -height; y < height; y++)
                drawPixel(x + cX, y + cY, color);
        }
    }

    static inline void drawLineRounded(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t thickness, uint32_t color) {
        thickness >>= 1;
        bool yLonger = false;
        int32_t shortLen = y2 - y1;
        int32_t longLen = x2 - x1;
        if (abs(shortLen) > abs(longLen)) {
            int32_t swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int32_t decInc;
        if (longLen == 0)
            decInc = 0;
        else
            decInc = (shortLen << 16) / longLen;

        if (yLonger) {
            if (longLen > 0)
            {
                longLen += y1;
                for (int32_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawCircle(j >> 16, y1, thickness, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int32_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawCircle(j >> 16, y1, thickness, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int32_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1)
            {
                drawCircle(x1, j >> 16, thickness, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int32_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawCircle(x1, j >> 16, thickness, color);
            j -= decInc;
        }
    }

    void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t thickness, uint32_t color) {
        // thickness = thickness || 1;
        #if !defined ENABLE_TURBO && !defined ENABLE_UNSAFE_NO_LOCKS
            pixels.take();
        #endif

        if (x1 == x2 && y1 == y2) {
            if (thickness == 1) {
                drawPixel(x1, y1, color);
            } else {
                drawCircle(x1, y1, thickness >> 1, color);
            }
        } else {
            if (thickness == 1) {
                drawLine1(x1, y1, x2, y2, color);
            } else {
                drawLineRounded(x1, y1, x2, y2, thickness, color);
            }
        }

        hasChanges = true;

        #if !defined ENABLE_TURBO && !defined ENABLE_UNSAFE_NO_LOCKS
            pixels.release();
        #endif
    }

    void stamp(int32_t x, int32_t y, SDL_Surface * surface) {
        #if !defined ENABLE_TURBO && !defined ENABLE_UNSAFE_NO_LOCKS
            pixels.take();
        #endif

        const int32_t surDrawW = (canvasWidth - x - surface->clip_rect.w) < 0 ? canvasWidth - x : surface->clip_rect.w;
        const int32_t surDrawH = (canvasHeight - y - surface->clip_rect.h) < 0 ? canvasHeight - y : surface->clip_rect.h;

        if (surDrawW <= 0 || surDrawH <= 0 || -x >= surface->clip_rect.w || -y >= surface->clip_rect.h)
            return;

        const int32_t surStartX = x < 0 ? -x : 0;
        const int32_t surStartY = y < 0 ? -y : 0;

        x = MAX_UNSAFE(x, 0);
        y = MAX_UNSAFE(y, 0);

        auto surP = (uint32_t *)surface->pixels + surStartY * surface->clip_rect.w + surStartX;
        const uint64_t surSkip = surface->clip_rect.w - surDrawW + surStartX;
        auto surEnd = (uint32_t *)surface->pixels + (surStartY + surDrawH - 1) * surface->clip_rect.w + surDrawW;
        
        auto canvasP = (uint32_t *)pixelBuffer + y * canvasWidth + x;
        const uint64_t canvasSkip = canvasWidth - surDrawW;

        while(surP < surEnd) {
            for (uint32_t sRowI = surStartX; sRowI < surDrawW; sRowI++, surP++, canvasP++) {
                const uint32_t surPix = *surP;
                const uint8_t alpha = (surPix >> 24);

                if (alpha == 0xFF) {
                    *canvasP = surPix;
                } else {
                    const uint32_t bgColor = *canvasP;

                    uint32_t rb = bgColor & 0x00FF00FF;
                    uint32_t g = bgColor & 0x0000FF00;
                    rb += ((surPix & 0xFF00FF) - rb) * alpha >> 8;
                    g += ((surPix & 0x00FF00) - g) * alpha >> 8;

                    *canvasP = (rb & 0xFF00FF) | (g & 0x00FF00);
                }

            }
            surP += surSkip;
            canvasP += canvasSkip;
        }

        #if !defined ENABLE_TURBO && !defined ENABLE_UNSAFE_NO_LOCKS
            pixels.release();
        #endif
    }
};

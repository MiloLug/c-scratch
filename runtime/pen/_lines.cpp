#include "_lines.h"
#include "_fig.h"
#include "_pixel.h"
#include "runtime/math.h"
#include <immintrin.h>


namespace Pen {
    static inline void drawLine2(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint64_t color) {
        bool yLonger = false;
        int64_t shortLen = y2 - y1;
        int64_t longLen = x2 - x1;
        if (abs(shortLen) > abs(longLen)) {
            int64_t swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int64_t decInc;
        if (longLen == 0)
            decInc = 0;
        else
            decInc = (shortLen << 16) / longLen;

        if (yLonger) {
            if (longLen > 0)
            {
                longLen += y1;
                for (int64_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawSquare2(j >> 16, y1, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int64_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawSquare2(j >> 16, y1, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int64_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1)
            {
                drawSquare2(x1, j >> 16, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int64_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawSquare2(x1, j >> 16, color);
            j -= decInc;
        }
    }

    static inline void drawHorizontalLine1(int64_t x1, int64_t x2, int64_t y, uint32_t color) {
        if (y < 0 || y >= canvasHeight) return;

        if (x1 > x2) {
            auto tmp = x1;
            x1 = x2;
            x2 = x1;
        }
        x1 = MIN(MAX_UNSAFE(x1, 0), canvasWidth - 1);
        x2 = MIN_UNSAFE(x2, canvasWidth - 1);

        auto rowStart = pixelBuffer + canvasWidth * y + x1;
        auto rowEnd = pixelBuffer + canvasWidth * y + x2;
        const uint8_t alpha = (color >> 24);

        #if defined(__AVX2__)
            __m256i colorVec = _mm256_set1_epi32(color);
            __m256i alphaVec = _mm256_set1_epi32(alpha);

            while (rowStart + 8 <= rowEnd) {
                __m256i rbMask = _mm256_set1_epi32(0x00FF00FF);
                __m256i gMask = _mm256_set1_epi32(0x0000FF00);

                __m256i bgColorVec = _mm256_loadu_si256((__m256i*)(rowStart));
                __m256i rbVec = bgColorVec & rbMask;
                __m256i gVec = bgColorVec & gMask;
                __m256i colorRbVec = colorVec & rbMask;
                __m256i colorGVec = colorVec & gMask;

                rbVec += _mm256_mullo_epi32(colorRbVec - rbVec, alphaVec) >> 8;
                gVec += _mm256_mullo_epi32(colorGVec - gVec, alphaVec) >> 8;

                _mm256_storeu_si256((__m256i*)(rowStart), (rbVec & rbMask) | (gVec & gMask));
                
                rowStart += 8;
            }
        #elif defined(__AVX__)
            __m128i colorVec = _mm_set1_epi32(color);
            __m128i alphaVec = _mm_set1_epi32(alpha);

            while (rowStart + 4 <= rowEnd) {
                __m128i rbMask = _mm_set1_epi32(0x00FF00FF);
                __m128i gMask = _mm_set1_epi32(0x0000FF00);

                __m128i bgColorVec = _mm_loadu_si128((__m128i*)(rowStart));
                __m128i rbVec = bgColorVec & rbMask;
                __m128i gVec = bgColorVec & gMask;
                __m128i colorRbVec = colorVec & rbMask;
                __m128i colorGVec = colorVec & gMask;

                rbVec += _mm_mullo_epi32(colorRbVec - rbVec, alphaVec) >> 8;
                gVec += _mm_mullo_epi32(colorGVec - gVec, alphaVec) >> 8;

                _mm_storeu_si128((__m128i*)(rowStart), (rbVec & rbMask) | (gVec & gMask));
                
                rowStart += 4;
            }
        #endif

        if (rowStart <= rowEnd) {
            if (alpha == 0xFF) {
                for (; rowStart <= rowEnd; rowStart++)
                    *rowStart = color;
            } else {
                for (; rowStart <= rowEnd; rowStart++) {
                    const uint32_t bgColor = *rowStart;

                    uint32_t rb = bgColor & 0x00FF00FF;
                    uint32_t g = bgColor & 0x0000FF00;
                    rb += ((color & 0xFF00FF) - rb) * alpha >> 8;
                    g += ((color & 0x00FF00) - g) * alpha >> 8;

                    *rowStart = (rb & 0xFF00FF) | (g & 0x00FF00);
                }
            }
        }
    }

    static inline void drawLine1(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint64_t color) {
        bool yLonger = false;
        int64_t shortLen = y2 - y1;
        int64_t longLen = x2 - x1;
        if (abs(shortLen) > abs(longLen)) {
            int64_t swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int64_t decInc;
        if (longLen == 0)
            decInc = 0;
        else
            decInc = (shortLen << 16) / longLen;

        if (yLonger) {
            if (longLen > 0)
            {
                longLen += y1;
                for (int64_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawPixel(j >> 16, y1, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int64_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawPixel(j >> 16, y1, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int64_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1)
            {
                drawPixel(x1, j >> 16, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int64_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawPixel(x1, j >> 16, color);
            j -= decInc;
        }
    }

    void drawLineRounded(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t d, uint64_t color) {
        d >>= 1;
        bool yLonger = false;
        int64_t shortLen = y2 - y1;
        int64_t longLen = x2 - x1;
        if (abs(shortLen) > abs(longLen)) {
            int64_t swap = shortLen;
            shortLen = longLen;
            longLen = swap;
            yLonger = true;
        }
        int64_t decInc;
        if (longLen == 0)
            decInc = 0;
        else
            decInc = (shortLen << 16) / longLen;

        if (yLonger) {
            if (longLen > 0)
            {
                longLen += y1;
                for (int64_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawCircle(j >> 16, y1, d, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int64_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawCircle(j >> 16, y1, d, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int64_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1)
            {
                drawCircle(x1, j >> 16, d, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int64_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawCircle(x1, j >> 16, d, color);
            j -= decInc;
        }
    }

    void drawLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint64_t d, uint64_t color) {
        if (!d) return;

        if (y1 == y2) {
            if (x1 == x2)
                switch (d) {
                    case 1:
                        drawPixel(x1, y1, color);
                        break;
                    case 2:
                        drawSquare2(x1, y1, color);
                        break;
                    default:
                        drawCircle(x1, y1, d >> 1, color);
                }
            else
                switch (d) {
                    case 1:
                        drawHorizontalLine1(x1, x2, y1, color);
                        break;
                    case 2:
                        drawLine2(x1, y1, x2, y2, color);
                        break;
                    default:
                        drawLineRounded(x1, y1, x2, y2, d, color);
                }
        } else
            switch (d) {
                case 1:
                    drawLine1(x1, y1, x2, y2, color);
                    break;
                case 2:
                    drawLine2(x1, y1, x2, y2, color);
                    break;
                default:
                    drawLineRounded(x1, y1, x2, y2, d, color);
            }
    }
}

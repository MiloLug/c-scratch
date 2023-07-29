#include "_lines.h"

#include "_fig.h"
#include "runtime/math/math.h"


#define __Pen_branch_lines_unsafe(__x1, __y1, __x2, __y2, __dot, __vertical, __horizontal, __free) \
    if (__x1 == __x2) {                                                                            \
        if (__y1 == __y2) {                                                                        \
            __dot;                                                                                 \
        } else {                                                                                   \
            __vertical;                                                                            \
        }                                                                                          \
    } else if (__y1 == __y2) {                                                                     \
        __horizontal;                                                                              \
    } else {                                                                                       \
        __free;                                                                                    \
    }


namespace Pen {
    static inline void drawLine2Segment(int64_t x, int64_t y, uint32_t color) {
        if (y < 0 || y >= canvasHeight) return;

        auto rowStart = pixelBuffer + canvasWidth * y + x;
        const uint32_t alpha = color & 0xFF;

        if (x < canvasWidth) {
            if (x > 0) {
                __Pen_drawPixelOnPointer_unsafe(rowStart, color, alpha);
            }
            if (x + 1 > 0) {
                __Pen_drawPixelOnPointer_unsafe(rowStart + 1, color, alpha);
            }
        }
    }

    static inline void drawLine2Helper(int32_t x, int32_t y, uint32_t color) {
        drawLine2Segment(x, y, color);
        drawLine2Segment(x, y + 1, color);
    }

    static void drawLine2(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t color) {
        bool yLonger = false;
        int64_t shortLen = y2 - y1;
        int64_t longLen = x2 - x1;
        if (std::abs(shortLen) > std::abs(longLen)) {
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
            if (longLen > 0) {
                longLen += y1;
                for (int64_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawLine2Helper(j >> 16, y1, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int64_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawLine2Helper(j >> 16, y1, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int64_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1) {
                drawLine2Helper(x1, j >> 16, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int64_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawLine2Helper(x1, j >> 16, color);
            j -= decInc;
        }
    }

    static inline void drawVerticalLine1(int64_t y1, int64_t y2, int64_t x, uint32_t color) {
        if (x < 0 || x >= canvasWidth) return;
        
        if (y1 > y2) {
            auto tmp = y1;
            y1 = y2;
            y2 = tmp;
        } 

        y1 = MIN(MAX_UNSAFE(y1, 0), canvasHeight - 1);
        y2 = MIN_UNSAFE(y2, canvasHeight - 1);

        auto rowStart = pixelBuffer + canvasWidth * y1 + x;
        auto rowEnd = pixelBuffer + canvasWidth * y2 + x;
        const uint32_t alpha = color & 0xFF;

        if (alpha == 0xFF) {
            for (; rowStart <= rowEnd; rowStart += canvasWidth) *rowStart = color;
            return;
        }

        for (; rowStart <= rowEnd; rowStart += canvasWidth) {
            __Pen_drawPixelOnPointer_unsafe(rowStart, color, alpha);
        }
    }

    void drawVerticalLine2(int64_t y1, int64_t y2, int64_t x, uint32_t color) {
        drawVerticalLine1(y1, y2, x, color);
        drawVerticalLine1(y1, y2, x + 1, color);
    }

    void drawHorizontalLine1(int64_t x1, int64_t x2, int64_t y, uint32_t color) {
        if (y < 0 || y >= canvasHeight) return;

        if (x1 > x2) {
            auto tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        x1 = MIN(MAX_UNSAFE(x1, 0), canvasWidth - 1);
        x2 = MIN_UNSAFE(x2, canvasWidth - 1);

        auto rowStart = pixelBuffer + canvasWidth * y + x1;
        auto rowEnd = pixelBuffer + canvasWidth * y + x2;
        const uint32_t srcA = color & 0xFF;

        if (srcA == 0xFF) {
            for (; rowStart <= rowEnd; rowStart++) *rowStart = color;
            return;
        }

#if defined(__AVX2__)
        if (rowStart + 8 <= rowEnd) {
            __m256i colorVec = _mm256_set1_epi32(color);
            __m256i alphaVec = _mm256_set1_epi32(srcA);
            __m256i t = _mm256_set1_epi32(0xFF);
            __m256i rbMask = _mm256_set1_epi32(0x00FF00FF);
            __m256i gMask = _mm256_set1_epi32(0x00FF0000);
            __m256i aMask = _mm256_set1_epi32(0x000000FF);

            while (rowStart + 8 <= rowEnd) {
                __m256i bgColorVec = _mm256_loadu_si256((__m256i *)(rowStart));
                __m256i resAVec = _mm256_add_epi32(
                    alphaVec,
                    _mm256_srli_epi32(_mm256_mullo_epi32(bgColorVec & aMask, t - alphaVec), 8)
                );

                __m256i rbVec = (bgColorVec >> 8) & rbMask;
                __m256i gVec = bgColorVec & gMask;

                rbVec += _mm256_mullo_epi32((colorVec >> 8 & rbMask) - rbVec, alphaVec) >> 8;
                gVec += _mm256_mullo_epi32((colorVec & gMask) - gVec, alphaVec) >> 8;

                _mm256_storeu_si256(
                    (__m256i *)(rowStart), (rbVec & rbMask) << 8 | (gVec & gMask) | resAVec
                );

                rowStart += 8;
            }
        }
#elif defined(__AVX__)
        if (rowStart + 4 <= rowEnd) {
            __m128i colorVec = _mm_set1_epi32(color);
            __m128i alphaVec = _mm_set1_epi32(srcA);
            __m128i t = _mm_set1_epi32(0xFF);
            __m128i rbMask = _mm_set1_epi32(0x00FF00FF);
            __m128i gMask = _mm_set1_epi32(0x00FF0000);
            __m128i aMask = _mm_set1_epi32(0x000000FF);

            while (rowStart + 4 <= rowEnd) {
                __m128i bgColorVec = _mm_loadu_si128((__m128i *)(rowStart));
                __m128i resAVec = _mm_add_epi32(
                    alphaVec, _mm_srli_epi32(_mm_mullo_epi32(bgColorVec & aMask, t - alphaVec), 8)
                );

                __m128i rbVec = (bgColorVec >> 8) & rbMask;
                __m128i gVec = bgColorVec & gMask;

                rbVec += _mm_mullo_epi32((colorVec >> 8 & rbMask) - rbVec, alphaVec) >> 8;
                gVec += _mm_mullo_epi32((colorVec & gMask) - gVec, alphaVec) >> 8;

                _mm_storeu_si128(
                    (__m128i *)(rowStart), (rbVec & rbMask) << 8 | (gVec & gMask) | resAVec
                );

                rowStart += 4;
            }
        }
#elif defined(__ARM_NEON__)
        constexpr uint32_t tRbMask = 0x00FF00FF, tGMask = 0x00FF0000, tAMask = 0x000000FF;

        if (rowStart + 16 <= rowEnd) {
            uint32x4_t colorVec = vld1q_dup_u32(&color);
            uint32x4_t alphaVec = vld1q_dup_u32(&srcA);
            
            uint32x4_t rbMask = vld1q_dup_u32(&tRbMask);
            uint32x4_t gMask = vld1q_dup_u32(&tGMask);
            uint32x4_t aMask = vld1q_dup_u32(&tAMask);

            while (rowStart + 16 <= rowEnd) {
                uint32x4x4_t bgColorVec = vld1q_u32_x4(rowStart);

                for (int i = 0; i < 4; i++) {
                    auto & tmpBgVec = bgColorVec.val[i];

                    uint32x4_t resAVec = vaddq_u32(
                        alphaVec,
                        vshrq_n_u32(
                            vmulq_u32(vandq_u32(tmpBgVec, aMask), vsubq_u32(aMask, alphaVec)),
                            8
                        )
                    );

                    uint32x4_t rbVec = vandq_u32(vshrq_n_u32(tmpBgVec, 8), rbMask);
                    uint32x4_t gVec = vandq_u32(tmpBgVec, gMask);

                    rbVec = vaddq_u32(
                        rbVec,
                        vshrq_n_u32(
                            vmulq_u32(
                                vsubq_u32(vandq_u32(vshrq_n_u32(colorVec, 8), rbMask), rbVec),
                                alphaVec
                            ),
                            8
                        )
                    );
                    gVec = vaddq_u32(
                        gVec,
                        vshrq_n_u32(
                            vmulq_u32(vsubq_u32(vandq_u32(colorVec, gMask), gVec), alphaVec),
                            8
                        )
                    );

                    tmpBgVec = vorrq_u32(
                        vshlq_n_u32(vandq_u32(rbVec, rbMask), 8),
                        vorrq_u32(
                            vandq_u32(gVec, gMask),
                            resAVec
                        )
                    );
                }

                vst1q_u32_x4(rowStart, bgColorVec);

                rowStart += 16;
            }
        }
#endif

        if (rowStart <= rowEnd) {
            for (; rowStart <= rowEnd; rowStart++) {
                __Pen_drawPixelOnPointer_unsafe(rowStart, color, srcA);
            }
        }
    }

    void drawHorizontalLine2(int64_t x1, int64_t x2, int64_t y, uint32_t color) {
        drawHorizontalLine1(x1, x2, y, color);
        drawHorizontalLine1(x1, x2, y + 1, color);
    }

    void drawLine1(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t color) {
        bool yLonger = false;
        int64_t shortLen = y2 - y1;
        int64_t longLen = x2 - x1;
        if (std::abs(shortLen) > std::abs(longLen)) {
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
            if (longLen > 0) {
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
            for (int64_t j = 0x8000 + (y1 << 16); x1 <= longLen; ++x1) {
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

    void drawSquareLineHelper(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint32_t color) {
        bool yLonger = false;
        int64_t shortLen = y2 - y1;
        int64_t longLen = x2 - x1;
        if (std::abs(shortLen) > std::abs(longLen)) {
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
            if (longLen > 0) {
                longLen += y1;
                for (int64_t j = 0x8000 + (x1 << 16); y1 <= longLen; ++y1) {
                    drawLine2Segment(j >> 16, y1, color);
                    j += decInc;
                }
                return;
            }
            longLen += y1;
            for (int64_t j = 0x8000 + (x1 << 16); y1 >= longLen; --y1) {
                drawLine2Segment(j >> 16, y1, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += x1;
            for (int64_t j = 0x8000 + (y1 << 16); x1 <= longLen; x1++) {
                drawLine2Segment(x1, j >> 16, color);
                j += decInc;
            }
            return;
        }
        longLen += x1;
        for (int64_t j = 0x8000 + (y1 << 16); x1 >= longLen; --x1) {
            drawLine2Segment(x1, j >> 16, color);
            j -= decInc;
        }
    }

    /**
     * @param sx/sy: starting x/y points of the square base
     * @param dx/dy: offset of the second base of the square
     */
    static void drawSquare(
        int64_t sx1,
        int64_t sy1,
        int64_t sx2,
        int64_t sy2,
        int64_t dx,
        int64_t dy,
        uint64_t color
    ) {
        bool yLonger = false;
        int64_t shortLen = sy2 - sy1;
        int64_t longLen = sx2 - sx1;
        if (std::abs(shortLen) > std::abs(longLen)) {
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
            if (longLen > 0) {
                longLen += sy1;
                for (int64_t j = 0x8000 + (sx1 << 16); sy1 <= longLen; ++sy1) {
                    drawSquareLineHelper(j >> 16, sy1, (j >> 16) + dx, sy1 + dy, color);
                    j += decInc;
                }
                return;
            }
            longLen += sy1;
            for (int64_t j = 0x8000 + (sx1 << 16); sy1 >= longLen; --sy1) {
                drawSquareLineHelper(j >> 16, sy1, (j >> 16) + dx, sy1 + dy, color);
                j -= decInc;
            }
            return;
        }

        if (longLen > 0) {
            longLen += sx1;
            for (int64_t j = 0x8000 + (sy1 << 16); sx1 <= longLen; ++sx1) {
                drawSquareLineHelper(sx1, j >> 16, sx1 + dx, (j >> 16) + dy, color);
                j += decInc;
            }
            return;
        }
        longLen += sx1;
        for (int64_t j = 0x8000 + (sy1 << 16); sx1 >= longLen; --sx1) {
            drawSquareLineHelper(sx1, j >> 16, sx1 + dx, (j >> 16) + dy, color);
            j -= decInc;
        }
    }

    void drawLineRounded(
        int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t d, uint32_t color
    ) {
        d = (d / 2) * 2 + 1;

        /* Calculate offsets for sides */
        double dx = (double)(x2 - x1);
        double dy = (double)(y2 - y1);
        double l = sqrt(dx*dx + dy*dy);
        double ang = atan2(dx, dy);
        double adj = 0.1 + 0.9 * fabs(radCos(2.0 * ang));
        double wl2 = ((double)d - adj)/(2.0 * l);
        double nx = dx * wl2;
        double ny = dy * wl2;

        double dx1 = (double)x1;
        double dy1 = (double)y1;

        drawCircle(x1, y1, d / 2, color);
        drawCircle(x2, y2, d / 2, color);

        drawSquare(
            dx1 + ny,
            dy1 - nx,
            dx1 - ny,
            dy1 + nx,
            dx,
            dy,
            color
        );
    }

    /// @brief common function for drawing lines
    /// @param x1 start X
    /// @param y1 start Y
    /// @param x2 end X
    /// @param y2 end Y
    /// @param thickness thickness in pixels
    /// @param color RGBA color
    void drawLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, uint64_t thickness, uint32_t color) {
        if (!thickness) return;

        switch (thickness) {
            case 1:
                __Pen_branch_lines_unsafe(
                    x1,
                    y1,
                    x2,
                    y2,
                    drawPixel(x1, y1, color),
                    drawVerticalLine1(y1, y2, x1, color),
                    drawHorizontalLine1(x1, x2, y1, color),
                    drawLine1(x1, y1, x2, y2, color)
                );
                break;
            case 2:
                __Pen_branch_lines_unsafe(
                    x1,
                    y1,
                    x2,
                    y2,
                    drawLine2Helper(x1, y1, color),
                    drawVerticalLine2(y1, y2, x1, color),
                    drawHorizontalLine2(x1, x2, y1, color),
                    drawLine2(x1, y1, x2, y2, color)
                );
                break;
            default:
                __Pen_branch_lines_unsafe(
                    x1,
                    y1,
                    x2,
                    y2,
                    drawCircle(x1, y1, thickness >> 1, color),
                    drawLineRounded(x1, y1, x2, y2, thickness, color),
                    drawLineRounded(x1, y1, x2, y2, thickness, color),
                    drawLineRounded(x1, y1, x2, y2, thickness, color)
                );
                break;
        }
    }
}

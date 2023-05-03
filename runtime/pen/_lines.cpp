#include "_lines.h"
#include "_fig.h"
#include "_pixel.h"


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

        if (x1 == x2 && y1 == y2) {
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
        } else {
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
}

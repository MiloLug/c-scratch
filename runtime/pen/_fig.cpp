#include "config.h"
#include "_fig.h"
#include "_pixel.h"
#include "runtime/math.h"


namespace Pen {
    void drawSquare2(int32_t x, int32_t y, uint32_t color) {
        drawPixel(x, y, color);
        drawPixel(x + 1, y, color);
        drawPixel(x + 1, y + 1, color);
        drawPixel(x, y + 1, color);
    }

    static inline void draw8SymmetryAA(int32_t cX, int32_t cY, int32_t x, int32_t y, int32_t color, uint32_t colorAA) {
        // AA color outer circle
        drawPixel(cX + x, cY + y, colorAA);
        drawPixel(cX + x, cY - y, colorAA);
        if (x != 0) {  // No repeating on top/bottom
            drawPixel(cX - x, cY + y, colorAA);
            drawPixel(cX - x, cY - y, colorAA);
        }
        if (x != y) { // No repeating on corners (45 deg)
            drawPixel(cX + y, cY + x, colorAA);
            drawPixel(cX - y, cY + x, colorAA);
            if (x != 0) { // No repeating on left/right sides
                drawPixel(cX + y, cY - x, colorAA);
                drawPixel(cX - y, cY - x, colorAA);
            }
        }
        
        y--;

        // normal color inner circle
        if ((x - 1) != y) {  // to avoid repeating due to "shrinking" the AA circle
            drawPixel(cX + x, cY + y, color);
            drawPixel(cX + x, cY - y, color);

            if (x != 0) {
                drawPixel(cX - x, cY + y, color);
                drawPixel(cX - x, cY - y, color);
            }

            if (x != y) {
                drawPixel(cX + y, cY + x, color);
                drawPixel(cX - y, cY + x, color);
                if (x != 0) {
                    drawPixel(cX + y, cY - x, color);
                    drawPixel(cX - y, cY - x, color);
                }
            }
        }
    }

    void drawCircle(int32_t cX, int32_t cY, int32_t r, uint32_t color) {
        int32_t i = 0;
        int32_t j = r + 1;
        int32_t rr = r * r;

        double lastFadeAmount = 0;
        double fadeAmount = 0;
        int32_t fadeAmountI;

        const int32_t maxOpaque = color >> 24;
        const int32_t noAlphaColor = color & 0x00FFFFFF;

        while (i < j) {
            double height = sqrt(MAX(rr - i * i, 0.0));
            fadeAmount = (double)maxOpaque * (1.0 - (ceil(height) - height));

            if (fadeAmount > lastFadeAmount)
                j--;
            lastFadeAmount = fadeAmount;
            
            draw8SymmetryAA(cX, cY, i, j, color, noAlphaColor | ((int32_t)fadeAmount << 24));

            // Draw vertical filling
            if ((i + 1) < j) {
                if (i != 0)
                    for (double x = -j + 2; x < j - 1 ; x++) {
                        drawPixel(cX + x, cY + i, color);
                        drawPixel(cX + x, cY - i, color);
                    }
                else
                    for (double x = -j + 2; x < j - 1 ; x++)
                        drawPixel(cX + x, cY + i, color);
            }

            i++;
        }

        // Draw top and bottom parts
        int32_t sideSpan = i - 1;
        i = 0;
        while (i < r) {
            double lineLength = sqrt(MAX(rr - i * i, 0.0));

            if (i != 0)
                for (int32_t y = sideSpan; y < lineLength - 1; y++) {
                    drawPixel(cX + i, cY + y, color);
                    drawPixel(cX + i, cY - y, color);
                    drawPixel(cX - i, cY + y, color);
                    drawPixel(cX - i, cY - y, color);
                }
            else
                for (int32_t y = sideSpan; y < lineLength - 1; y++) {
                    drawPixel(cX + i, cY + y, color);
                    drawPixel(cX + i, cY - y, color);
                }

            i++;
        }
    }
}

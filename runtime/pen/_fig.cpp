#include "_fig.h"

#include "_lines.h"


namespace Pen {
    static inline void draw8Symmetry(int32_t cX, int32_t cY, int32_t x, int32_t y, int32_t color) {
        drawPixel(cX + x, cY + y, color);
        drawPixel(cX + x, cY - y, color);
        if (x != 0) {  // No repeating on top/bottom
            drawPixel(cX - x, cY + y, color);
            drawPixel(cX - x, cY - y, color);
        }
        if (x != y) {  // No repeating on corners (45 deg)
            drawPixel(cX + y, cY + x, color);
            drawPixel(cX - y, cY + x, color);
            if (x != 0) {  // No repeating on left/right sides
                drawPixel(cX + y, cY - x, color);
                drawPixel(cX - y, cY - x, color);
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

        const int32_t maxOpaque = color & 0xFF;
        const int32_t noAlphaColor = color & 0xFFFFFF00;

        while (i < j) {
            double height = sqrt(rr - i * i);
            fadeAmount = (double)maxOpaque * (1.0 - (ceil(height) - height));

            // If fade amount is dropping, then, obviously, it's a new step
            if (fadeAmount > lastFadeAmount) j--;
            lastFadeAmount = fadeAmount;

            // Draw the semi-transparent circle around the filling
            draw8Symmetry(cX, cY, i, j, noAlphaColor | ((int32_t)fadeAmount & 0xFF));

            // Draw central filling
            if (i != 0) {
                drawLine(cX - j + 1, cY + i, cX + j - 1, cY + i, 1, color);
                drawLine(cX - j + 1, cY - i, cX + j - 1, cY - i, 1, color);
            } else
                drawLine(cX - j + 1, cY + i, cX + j - 1, cY + i, 1, color);
            i++;
        }

        // Draw top and bottom parts
        while (i < r) {
            int32_t lineLength = ceil(sqrt(rr - i * i));

            if (lineLength) {
                drawLine(cX - lineLength + 1, cY + i, cX + lineLength - 1, cY + i, 1, color);
                drawLine(cX - lineLength + 1, cY - i, cX + lineLength - 1, cY - i, 1, color);
            }

            i++;
        }
    }
}  // namespace Pen

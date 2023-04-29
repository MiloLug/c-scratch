#include "_stamp.h"
#include "_pixel.h"
#include "runtime/math.h"


namespace Pen {
    void stamp(int32_t x, int32_t y, SDL_Surface * surface) {
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
    }
}

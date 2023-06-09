#include "_stamp.h"

#include "_pixel.h"
#include "runtime/math/common.h"
#include "runtime/utils.h"


namespace Pen {
    force_inline__ void stampDebug(uint32_t * pixel) {
        const uint32_t fill = 0x00FF00;
        const uint8_t alpha = 0x9F;

        const uint32_t bgColor = *pixel;
        const uint32_t resA = alpha + ((bgColor & 0xFF) * (255 - alpha) >> 8);

        uint32_t rb = bgColor >> 8 & 0x00FF00FF;
        uint32_t g = bgColor & 0x00FF0000;
        rb += ((fill >> 8 & 0x00FF00FF) - rb) * alpha >> 8;
        g += ((fill & 0x00FF0000) - g) * alpha >> 8;

        *pixel = (rb & 0x00FF00FF) << 8 | (g & 0x00FF0000) | resA;
    }

    void stamp(int64_t x, int64_t y, SDL_Surface * surface) {
        const int32_t surDrawW = (canvasWidth - x - surface->w) < 0 ? canvasWidth - x : surface->w;
        const int32_t surDrawH =
            (canvasHeight - y - surface->h) < 0 ? canvasHeight - y : surface->h;

        if (surDrawW <= 0 || surDrawH <= 0 || -x >= surface->w || -y >= surface->h) return;

        const int32_t surStartX = x < 0 ? -x : 0;
        const int32_t surStartY = y < 0 ? -y : 0;

        x = MAX_UNSAFE(x, 0);
        y = MAX_UNSAFE(y, 0);

        auto surP = (uint32_t *)surface->pixels + surStartY * surface->w + surStartX;
        const uint64_t surSkip = surface->w - surDrawW + surStartX;
        auto surEnd =
            (uint32_t *)surface->pixels + (surStartY + surDrawH - 1) * surface->w + surDrawW;

        auto canvasP = (uint32_t *)pixelBuffer + y * canvasWidth + x;
        const uint64_t canvasSkip = canvasWidth - surDrawW + surStartX;

        while (surP < surEnd) {
            for (uint32_t sRowI = surStartX; sRowI < surDrawW; sRowI++, surP++, canvasP++) {
                const uint32_t surPix = *surP;
                const uint8_t alpha = surPix & 0xFF;

                if (alpha == 0xFF) {
                    *canvasP = surPix;
                } else {
                    __Pen_drawPixelOnPointer_unsafe(canvasP, surPix, alpha);
                }

#ifdef PEN_STAMP_DEBUG
                stampDebug(canvasP);
#endif
            }
            surP += surSkip;
            canvasP += canvasSkip;
        }
    }
}

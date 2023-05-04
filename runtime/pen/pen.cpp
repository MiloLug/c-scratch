#include "pen.h"


namespace Pen
{
    Initializer::Initializer(SDL_Renderer *renderer) {
        if (!initialized) {
            initialized = true;
            texture = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STREAMING,
                WINDOW_WIDTH,
                WINDOW_HEIGHT
            );

            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

            memset((void *)pixelBuffer, 0x00, canvasSize << 2);
        }
    }
    Initializer::~Initializer() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        texture = NULL;
    }
    volatile bool Initializer::initialized = false;
};

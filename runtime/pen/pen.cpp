#include "pen.h"
#include <cstdint>
#include "_pixel.h"
#include "runtime/sdl.h"


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
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        texture = NULL;
    }
    volatile bool Initializer::initialized = false;
};

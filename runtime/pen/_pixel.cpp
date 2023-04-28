#include "config.h"
#include "_pixel.h"


namespace Pen {
    // 1 pixel = 4 values x 8bit = 32bit

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    volatile bool hasChanges = true;
    volatile uint32_t * pixelBuffer;
    Mutex pixels;
}

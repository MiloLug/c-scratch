#include "_pixel.h"


namespace Pen {
    SDL_Surface * surface = NULL;
    SDL_Texture * texture = NULL;
    volatile bool hasChanges = true;
    uint32_t pixelBuffer[canvasSize];
    Mutex pixels;
}

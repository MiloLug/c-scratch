#ifndef CSCRATCH_PEN_STAMP_H
#define CSCRATCH_PEN_STAMP_H

#include <cstdint>
#include "runtime/include_sdl.h"


namespace Pen {
    void stamp(int32_t x, int32_t y, SDL_Surface * surface);
}


#endif

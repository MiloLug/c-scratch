#ifndef CSCRATCH_PEN_STAMP_H
#define CSCRATCH_PEN_STAMP_H

#include "runtime/include_sdl.h"

#include <cstdint>


namespace Pen {
    void stamp(int64_t x, int64_t y, SDL_Surface * surface);
}


#endif

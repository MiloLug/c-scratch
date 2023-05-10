#ifndef CSCRATCH_PEN_H
#define CSCRATCH_PEN_H

#include "_fig.h"
#include "_lines.h"
#include "_pixel.h"
#include "_stamp.h"
#include "config.h"
#include "runtime/include_sdl.h"


namespace Pen {
    class Initializer {
    public:
        Initializer(SDL_Renderer * renderer);
        ~Initializer();

    private:
        static volatile bool initialized;
    };
};  // namespace Pen


#endif

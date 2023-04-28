#ifndef CSCRATCH_PEN_H
#define CSCRATCH_PEN_H

#include "config.h"
#include "runtime/sdl.h"
#include "_pixel.h"
#include "_fig.h"
#include "_lines.h"
#include "_stamp.h"


namespace Pen {
    class Initializer {
    public:
        Initializer(ScratchSDLWindow &window);
        ~Initializer();
    private:
        static volatile bool initialized;
    };
};


#endif

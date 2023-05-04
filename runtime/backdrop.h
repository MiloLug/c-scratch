#ifndef CSCRATCH_BACKDROP_H
#define CSCRATCH_BACKDROP_H

#include "config.h"
#include "sprite_base.h"


struct BackdropDeclaration {
    uint64_t costumeNumber;
};


class Backdrop: public SpriteBase {
public:
    Backdrop(const BackdropDeclaration &decl):
        SpriteBase(ASSETS_BASE_DIR / L"background", decl.costumeNumber - 1)
    {}
};


#endif

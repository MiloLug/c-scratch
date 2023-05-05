#ifndef SPRITE_H
#define SPRITE_H

#include "config.h"
#include "sprite_base.h"
#include <list>


struct SpriteDeclaration {
    const wchar_t * name;
    float x;
    float y;
    float width;
    float height;
    float direction;
    uint64_t costumeNumber;
    float size;
    bool visible;
    uint64_t layerOrder;
    const std::vector<Costume> costumes;
};


class Sprite: public Movable, public SpriteBase {
public:
    const wchar_t * name;
    SDL_Surface * surfaceCache = NULL;  // Surface cache for the Pen's stamp
    bool visible;
    uint64_t layerOrder;

    Sprite(const SpriteDeclaration &decl):
        Movable(decl.x, decl.y, decl.width, decl.height, decl.direction, decl.size),
        SpriteBase(
            ASSETS_BASE_DIR / L"sprites" / decl.name,
            decl.costumeNumber - 1,
            decl.costumes
        ),
        name{decl.name},
        visible{decl.visible},
        layerOrder{decl.layerOrder - 1}
    {}

    bool isTouchingPointer() {
        const auto tmpSurface = getCostumeTransformedSurface();

        const auto
            cOffsetX = tmpSurface->w / 2,
            cOffsetY = tmpSurface->h / 2;

        if (
            mouseState.x > (x + cOffsetX)
            || mouseState.x < (x - cOffsetX)
            || mouseState.y > (y + cOffsetY)
            || mouseState.y < (y - cOffsetY)
        )
            return false;
        
        const uint64_t
            pixelX = round((float)mouseState.x - x + cOffsetX),
            pixelY = round(y - mouseState.y + cOffsetY);
        
        return (((uint32_t *)tmpSurface->pixels)[tmpSurface->w * pixelY + pixelX] >> 24) != 0;
    }

    SDL_Surface * getCostumeTransformedSurface() {
        if (!shouldUpdateSurfaceCache) return surfaceCache;
        
        if (surfaceCache != NULL)
            SDL_FreeSurface(surfaceCache);
        surfaceCache = rotozoomSurface(getCostumeSurface(), -direction, 1, 1);
        
        return surfaceCache;
    }

    void penStamp() {
        auto tmp = getCostumeTransformedSurface();

        Pen_safe(Pen::stamp(
            round(WINDOW_CENTER_X - (float)tmp->w / 2.0 + x),
            round(WINDOW_CENTER_Y - (float)tmp->h / 2.0 - y),
            tmp
        ));
    }

    ~Sprite() {
        if (surfaceCache)
            SDL_FreeSurface(surfaceCache);
    }
};


#endif

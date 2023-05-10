#ifndef SPRITE_H
#define SPRITE_H

#include "actions.h"
#include "config.h"
#include "sprite_base.h"

#include <list>


struct SpriteDeclaration {
    const wchar_t * name;
    const wchar_t * safeName;
    float x;
    float y;
    float width;
    float height;
    float direction;
    uint64_t costumeIndex;
    float size;
    bool visible;
    uint64_t layerOrder;
    const std::vector<Costume> costumes;
};


class Sprite: public Movable, public SpriteBase {
public:
    const wchar_t * name;
    const wchar_t * safeName;
    const uint64_t id;
    const uint64_t actionId;
    SDL_Surface * surfaceCache = NULL;  // Surface cache for the Pen's stamp
    bool visible;
    uint64_t layerOrder;

    Sprite(const SpriteDeclaration & decl):
        Movable(decl.x, decl.y, decl.width, decl.height, decl.direction, decl.size),
        SpriteBase(
            ASSETS_BASE_DIR / L"sprites" / decl.safeName, decl.costumeIndex - 1, decl.costumes
        ),
        name{decl.name},
        safeName{decl.safeName},
        visible{decl.visible},
        layerOrder{decl.layerOrder - 1},
        id{fastHash(decl.name)},
        actionId{id & ~ACTION_ID_MASK} {}

    bool isTouchingXY(float x1, float y1) {
        const auto tmpSurface = getCostumeTransformedSurface();

        const auto cOffsetX = tmpSurface->w / 2, cOffsetY = tmpSurface->h / 2;

        if (x1 > (x + cOffsetX) || x1 < (x - cOffsetX) || y1 > (y + cOffsetY) ||
            y1 < (y - cOffsetY))
            return false;

        const uint64_t pixelX = round(x1 - x + cOffsetX), pixelY = round(y - y1 + cOffsetY);

        return (((uint32_t *)tmpSurface->pixels)[tmpSurface->w * pixelY + pixelX] >> 24) != 0;
    }

    SDL_Surface * getCostumeTransformedSurface() {
        if (!shouldUpdateSurfaceCache) return surfaceCache;

        if (surfaceCache != NULL) SDL_FreeSurface(surfaceCache);
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

    void hide() { visible = false; }
    void show() { visible = true; }

    ~Sprite() {
        if (surfaceCache) SDL_FreeSurface(surfaceCache);
    }
};


#endif

#ifndef SPRITE_H
#define SPRITE_H

#include "config.h"
#include "runtime/script/actions.h"
#include "sprite_base.h"

#include <list>


struct SpriteDeclaration {
    const wchar_t * name;
    const wchar_t * safeName;
    float x;
    float y;
    float direction;
    uint64_t costumeIndex;
    float size;
    bool visible;
    uint64_t layerOrder;
    const std::vector<Costume> costumes;
};


class Sprite: public Movable, public SpriteBase {
protected:
    force_inline__ void onCostumeSwitch() {
        if (currentCostume != &costumes[costumeIndex]) {
            currentCostume = &costumes[costumeIndex];

            pPointOrig.x = currentCostume->pX;
            pPointOrig.y = currentCostume->pY;
            wOrig = currentCostume->w;
            hOrig = currentCostume->h;

            updateOffsetsAndPoints();
        }
    }

public:
    const wchar_t * name;
    const wchar_t * safeName;
    const uint64_t id;
    const uint64_t actionId;
    SDL_Surface * surfaceCache = NULL;  // Surface cache for the Pen's stamp
    bool visible;
    uint64_t layerOrder;

    Sprite(const SpriteDeclaration & decl):
        Movable(
            decl.x,
            decl.y,
            decl.costumes[decl.costumeIndex - 1].pX,
            decl.costumes[decl.costumeIndex - 1].pY,
            decl.costumes[decl.costumeIndex - 1].w,
            decl.costumes[decl.costumeIndex - 1].h,
            decl.direction,
            decl.size
        ),
        SpriteBase(ASSETS_BASE_DIR / L"sprites" / decl.safeName, decl.costumeIndex, decl.costumes),
        name{decl.name},
        safeName{decl.safeName},
        visible{decl.visible},
        layerOrder{decl.layerOrder - 1},
        id{fastHash(decl.name)},
        actionId{id & ~ACTION_ID_MASK} {}

    bool isTouchingXY(float x1, float y1) {
        const auto tmpSurface = getCostumeTransformedSurface();

        x1 = (float)tmpSurface->w / 2.0 + (x1 - x) + pUnrotatedPoint.x - 1.0f;
        y1 = (float)tmpSurface->h / 2.0 + (y - y1) - pUnrotatedPoint.y - 1.0f;

        if (x1 < 0 || y1 < 0 || x1 >= tmpSurface->w || y1 >= tmpSurface->h) return false;

        return (((uint32_t *)tmpSurface->pixels)[(uint64_t)(tmpSurface->w * round(y1) + round(x1))]
                & 0xFF)
            != 0;
    }

    SDL_Surface * getCostumeTransformedSurface() {
        if (!shouldUpdateTransformCache) return surfaceCache;

        if (surfaceCache != NULL) SDL_FreeSurface(surfaceCache);
        auto src = getCostumeSurface();
        surfaceCache = rotozoomSurface(src, -direction, pos.h / src->h, 1);

        return surfaceCache;
    }

    void penStamp() {
        auto tmp = getCostumeTransformedSurface();

        Pen_safe(Pen::stamp(
            round(WINDOW_CENTER_X - (float)tmp->w / 2.0 + x - pUnrotatedPoint.x),
            round(WINDOW_CENTER_Y - (float)tmp->h / 2.0 - y + pUnrotatedPoint.y),
            tmp
        ));
    }

    force_inline__ void hide() { visible = false; }
    force_inline__ void show() { visible = true; }

    ~Sprite() {
        if (surfaceCache) SDL_FreeSurface(surfaceCache);
    }
};


#endif

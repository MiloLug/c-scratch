#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <list>
#include <set>
#include "include_sdl.h"
#include "sprite.h"


class SpriteManager {
public:
    typedef std::list<Sprite *> SpriteList;

protected:
    static SpriteList spriteStorage;
    static std::set<Sprite *> managedSprites;

public:
    static void renderSprites(SDL_Renderer * renderer);
    static void initSprites(SDL_Renderer * renderer, const SpriteList &sprites);

    static void moveByLayers(Sprite * sprite, int64_t layersNumber);
    static void moveBackward(Sprite * sprite, int64_t layersNumber);
    static void moveForward(Sprite * sprite, int64_t layersNumber);
    static void moveToBack(Sprite * sprite);
    static void moveToFront(Sprite * sprite);
};


#endif
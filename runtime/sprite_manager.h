#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <list>
#include <unordered_set>
#include <memory>
#include "include_sdl.h"
#include "sprite.h"
#include "backdrop.h"


class SpriteManager {
public:
    typedef std::list<Sprite *> SpriteList;

protected:
    static SpriteList spriteStorage;
    static constinit std::unique_ptr<SpriteList> waitingForInit;
    static std::unordered_set<Sprite *> managedSprites;
    static Backdrop * backdrop;

public:
    static void add(Sprite * sprite);

    static void renderBackdrop(SDL_Renderer * renderer);
    static void initBackdrop(SDL_Renderer * renderer);

    static void renderSprites(SDL_Renderer * renderer);
    static void initSprites(SDL_Renderer * renderer);

    static void moveByLayers(Sprite * sprite, int64_t layersNumber);
    static void moveBackward(Sprite * sprite, int64_t layersNumber);
    static void moveForward(Sprite * sprite, int64_t layersNumber);
    static void moveToBack(Sprite * sprite);
    static void moveToFront(Sprite * sprite);

    static void staticInit();

    SpriteManager(Sprite * sprite);
    SpriteManager(Backdrop * _backdrop);
};


#endif

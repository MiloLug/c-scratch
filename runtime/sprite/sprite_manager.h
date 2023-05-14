#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <list>
#include <memory>
#include <unordered_set>


class Sprite;
class Backdrop;
class SDL_Renderer;


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

    static Sprite * getTouchingXY(float x, float y);
    static void sendClickXY(float x, float y);

    static void staticInit();

    SpriteManager(Sprite * sprite);
    SpriteManager(Backdrop * _backdrop);
};


#endif

#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sdl.h"
#include "sprite.h"


inline void renderSprites(ScratchSDLWindow &window, Sprite * const sprites[]) {
    SDL_RenderClear(window.renderer);
    const Sprite * sprite;
    int i = 0;
    while ((sprite = sprites[i++]) != NULL) {
        SDL_RenderCopyExF(window.renderer, sprite->getCostumeTexture(), NULL, &sprite->pos, sprite->direction, NULL, SDL_FLIP_NONE);
    }
    SDL_RenderPresent(window.renderer);
}

inline void setCanMove(Sprite * const sprites[], bool value) {
    Sprite * sprite;
    int i = 0;
    while ((sprite = sprites[i++]) != NULL) {
        sprite->canMove = value;
    }
}

inline void initSprites(ScratchSDLWindow &window, Sprite * const sprites[]) {
    Sprite * sprite;
    int i = 0;
    while ((sprite = sprites[i++]) != NULL) {
        sprite->init(window.renderer);
    }
}

#endif
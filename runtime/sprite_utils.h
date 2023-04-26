#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sdl.h"
#include "sprite.h"


static inline void renderSprites(ScratchSDLWindow &window, Sprite * const sprites[]) {
    const Sprite * sprite;
    int i = 0;
    while ((sprite = sprites[i++]) != NULL) {
        SDL_RenderCopyExF(window.renderer, sprite->getCostumeTexture(), NULL, &sprite->pos, sprite->direction, NULL, SDL_FLIP_NONE);
    }
}

static inline void initSprites(ScratchSDLWindow &window, Sprite * const sprites[]) {
    Sprite * sprite;
    int i = 0;
    while ((sprite = sprites[i++]) != NULL) {
        sprite->init(window.renderer);
        
        #ifdef DEBUG
            wprintf(L"initSprites: initialized %ls\n", sprite->name);
        #endif
    }
}

#endif

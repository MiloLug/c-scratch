#include "sprite_utils.h"


SpritesList spritesStorage;


void renderSprites(ScratchSDLWindow &window) {
    for (auto &sprite : spritesStorage) {
        if (sprite->visible)
            SDL_RenderCopyExF(window.renderer, sprite->getCostumeTexture(), NULL, &sprite->pos, sprite->direction, NULL, SDL_FLIP_NONE);
    }
}

void initSprites(ScratchSDLWindow &window, const SpritesList &sprites) {
    for (auto &sprite : sprites) {
        sprite->init(window.renderer);
        spritesStorage.push_back(sprite);

        #ifdef DEBUG
            wprintf(L"Sprites initialization: %ls OK\n", sprite->name);
        #endif
    }
}

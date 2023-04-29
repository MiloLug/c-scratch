#include "sprite_utils.h"


SpritesList spritesStorage;


void renderSprites(SDL_Renderer *renderer) {
    for (auto &sprite : spritesStorage) {
        if (sprite->visible)
            SDL_RenderCopyExF(renderer, sprite->getCostumeTexture(), NULL, &sprite->pos, sprite->direction, NULL, SDL_FLIP_NONE);
    }
}

void initSprites(SDL_Renderer *renderer, const SpritesList &sprites) {
    for (auto &sprite : sprites) {
        sprite->init(renderer);
        spritesStorage.push_back(sprite);

        #ifdef DEBUG
            wprintf(L"Sprites initialization: %ls OK\n", sprite->name);
        #endif
    }
}

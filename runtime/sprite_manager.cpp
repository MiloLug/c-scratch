#include "sprite_manager.h"
#include "utils.h"


SpriteManager::SpriteList SpriteManager::spriteStorage;
std::map<const Sprite *, uint64_t> SpriteManager::spriteIndexes;


void SpriteManager::renderSprites(SDL_Renderer *renderer) {
    for (auto &sprite : spriteStorage) {
        if (sprite->visible)
            SDL_RenderCopyExF(renderer, sprite->getCostumeTexture(), NULL, &sprite->pos, sprite->direction, NULL, SDL_FLIP_NONE);
    }
}

void SpriteManager::initSprites(SDL_Renderer *renderer, const SpriteList &sprites) {
    for (auto &sprite : sprites) {
        sprite->init(renderer);
        spriteStorage.push_back(sprite);
        spriteIndexes[sprite] = spriteStorage.size() - 1;

        #ifdef DEBUG
            wprintf(L"Sprites initialization: %ls OK\n", sprite->name);
        #endif
    }
}

void SpriteManager::moveByLayers(Sprite * sprite, int64_t layersNumber) {
    auto found = spriteIndexes.find(sprite);

    if (found != spriteIndexes.end()) {
        const auto newLayer = MAX(MIN(found->second + layersNumber, spriteStorage.size() - 1), 0);
        const auto oldLayer = found->second;

        if (newLayer == oldLayer) return;

        auto listIter = spriteStorage.begin();
        uint64_t i = 0;
        found->second = newLayer;
            
        if (newLayer < oldLayer) {
            for (; i < newLayer; i++, listIter++);

            spriteStorage.insert(listIter, sprite);

            for (; i < oldLayer; i++, listIter++) {
                spriteIndexes[*listIter]++;
            }
            spriteStorage.erase(listIter);
        } else {
            for (; i < oldLayer; i++, listIter++);

            spriteStorage.erase(listIter++);

            for (; i < newLayer; i++, listIter++) {
                spriteIndexes[*listIter]--;
            }
            spriteStorage.insert(listIter, sprite);
        }
    }
}

void SpriteManager::moveBackward(Sprite * sprite, int64_t layersNumber) {
    moveByLayers(sprite, -layersNumber);
}
void SpriteManager::moveForward(Sprite * sprite, int64_t layersNumber) {
    moveByLayers(sprite, layersNumber);
}

void SpriteManager::moveToBack(Sprite * sprite) {
    moveByLayers(sprite, -spriteStorage.size());
}
void SpriteManager::moveToFront(Sprite * sprite) {
    moveByLayers(sprite, spriteStorage.size());
}

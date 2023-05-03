#include "sprite_manager.h"
#include "utils.h"


SpriteManager::SpriteList SpriteManager::spriteStorage;
std::set<Sprite *> SpriteManager::managedSprites;


void SpriteManager::renderSprites(SDL_Renderer *renderer) {
    for (auto &sprite : spriteStorage) {
        if (sprite->visible)
            SDL_RenderCopyExF(renderer, sprite->getCostumeTexture(), NULL, &sprite->pos, sprite->direction, NULL, SDL_FLIP_NONE);
    }
}

void SpriteManager::initSprites(SDL_Renderer *renderer, const SpriteList &sprites) {
    for (auto &sprite : sprites) {
        if (managedSprites.contains(sprite)) continue;

        sprite->init(renderer);
        managedSprites.insert(sprite);

        const auto maxLayer = spriteStorage.size();
        const auto layer = MAX(MIN(sprite->layerOrder - 1, maxLayer), 0);  // layerOrder starts from 1 on declaration
        auto listIter = spriteStorage.begin();
        uint64_t i = 0;

        sprite->layerOrder = layer;

        if (layer == maxLayer) {
            spriteStorage.push_back(sprite);
        } else if (layer == 0) {
            spriteStorage.push_front(sprite);
            for (; i < maxLayer; i++, listIter++) {
                (*listIter)->layerOrder++;
            }
        } else {
            for (; i < layer; i++, listIter++);

            spriteStorage.insert(listIter, sprite);

            for (; i < maxLayer; i++, listIter++) {
                (*listIter)->layerOrder++;
            }
            spriteStorage.erase(listIter);
        }
        
        #ifdef DEBUG
            wprintf(L"Sprites initialization: %ls OK\n", sprite->name);
        #endif
    }
}

void SpriteManager::moveByLayers(Sprite * sprite, int64_t layersNumber) {
    if (!managedSprites.contains(sprite)) return;

    const auto newLayer = MAX(MIN(sprite->layerOrder + layersNumber, spriteStorage.size() - 1), 0);
    const auto oldLayer = sprite->layerOrder;

    if (newLayer == oldLayer) return;

    auto listIter = spriteStorage.begin();
    uint64_t i = 0;
    sprite->layerOrder = newLayer;
        
    if (newLayer < oldLayer) {
        for (; i < newLayer; i++, listIter++);

        spriteStorage.insert(listIter, sprite);

        for (; i < oldLayer; i++, listIter++) {
            (*listIter)->layerOrder++;
        }
        spriteStorage.erase(listIter);
    } else {
        for (; i < oldLayer; i++, listIter++);

        spriteStorage.erase(listIter++);

        for (; i < newLayer; i++, listIter++) {
            (*listIter)->layerOrder--;
        }
        spriteStorage.insert(listIter, sprite);
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

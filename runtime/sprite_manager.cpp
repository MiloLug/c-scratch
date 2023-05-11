#include "sprite_manager.h"

#include "script_manager.h"
#include "utils.h"


SpriteManager::SpriteList SpriteManager::spriteStorage;
constinit std::unique_ptr<SpriteManager::SpriteList> SpriteManager::waitingForInit = nullptr;
std::unordered_set<Sprite *> SpriteManager::managedSprites;
constinit Backdrop * SpriteManager::backdrop = nullptr;


// STATIC

void SpriteManager::add(Sprite * sprite) { waitingForInit->push_back(sprite); }

void SpriteManager::renderBackdrop(SDL_Renderer * renderer) {
    if (backdrop) {
        SDL_RenderCopy(renderer, backdrop->getCostumeTexture(), NULL, NULL);
    }
}
void SpriteManager::initBackdrop(SDL_Renderer * renderer) {
    if (backdrop == nullptr) {
        wprintf(L"Backdrop initialization: FAILED, no backdrop\n");
        return;
    }
    backdrop->init(renderer);

#ifdef DEBUG
    wprintf(L"Backdrop initialization: OK\n");
#endif
}

void SpriteManager::renderSprites(SDL_Renderer * renderer) {
    for (auto & sprite : spriteStorage) {
        if (sprite->visible)
            SDL_RenderCopyExF(
                renderer,
                sprite->getCostumeTexture(),
                NULL,
                &sprite->getRectLT(),
                sprite->getDirectionLT(),
                &sprite->getPivotLT(),
                SDL_FLIP_NONE
            );
    }
}
void SpriteManager::initSprites(SDL_Renderer * renderer) {
    for (auto & sprite : *waitingForInit) {
        if (managedSprites.contains(sprite)) continue;

        sprite->init(renderer);
        managedSprites.insert(sprite);

        const auto maxLayer = spriteStorage.size();
        const auto layer =
            MAX(MIN(sprite->layerOrder - 1, maxLayer),
                0);  // layerOrder starts from 1 on declaration
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
            for (; i < layer; i++, listIter++)
                ;

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

    waitingForInit->clear();
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
        for (; i < newLayer; i++, listIter++)
            ;

        spriteStorage.insert(listIter, sprite);

        for (; i < oldLayer; i++, listIter++) {
            (*listIter)->layerOrder++;
        }
        spriteStorage.erase(listIter);
    } else {
        for (; i < oldLayer; i++, listIter++)
            ;

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

void SpriteManager::moveToBack(Sprite * sprite) { moveByLayers(sprite, -spriteStorage.size()); }
void SpriteManager::moveToFront(Sprite * sprite) { moveByLayers(sprite, spriteStorage.size()); }

void SpriteManager::staticInit() {
    if (waitingForInit != nullptr) return;

    waitingForInit.reset(new SpriteList);
}

Sprite * SpriteManager::getTouchingXY(float x, float y) {
    for (auto iter = spriteStorage.rbegin(), end = spriteStorage.rend(); iter != end; iter++) {
        auto sprite = *iter;
        if (sprite->visible && sprite->isTouchingXY(x, y)) {
            return sprite;
        }
    }

    return nullptr;
}

void SpriteManager::sendClickXY(float x, float y) {
    auto sprite = getTouchingXY(x, y);
    if (sprite != nullptr) {
        ScriptManager::triggerScripts(ACTION_CLICK | sprite->actionId);
    }
}

// NON-STATIC

SpriteManager::SpriteManager(Sprite * sprite) {
    staticInit();
    add(sprite);
}

SpriteManager::SpriteManager(Backdrop * _backdrop) { backdrop = _backdrop; }

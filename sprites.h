#ifndef SPRITES_COLLECTION_H
#define SPRITES_COLLECTION_H

#include "runtime/sprite.h"
#include "runtime/sprite_manager.h"


Sprite sprite({
    .name = L"sprite",
    .x = 100,
    .y = 0,
    .width = 95,
    .height = 100,
    .direction = 135,
    .costumeNumber = 1,
    .size = 100,
    .visible = false,
    .layerOrder = 1,
});

Sprite sprite2({
    .name = L"sprite2",
    .x = -79,
    .y = -27,
    .width = 95,
    .height = 100,
    .direction = 90,
    .costumeNumber = 1,
    .size = 100,
    .visible = false,
    .layerOrder = 1,
});

SpriteManager::SpriteList sprites = {
    &sprite2,
    &sprite
};


#endif

#ifndef SPRITES_COLLECTION_H
#define SPRITES_COLLECTION_H

#include "runtime/sprite.h"


Sprite sprite = {
    .name = L"sprite",
    .pos = {
        .x = (WINDOW_WIDTH - 95) / 2 + 100,
        .y = (WINDOW_HEIGHT - 100) / 2 - 0,
        .w = 95,
        .h = 100
    },
    .direction = 45,
    .costumeNumber = 0,
    .size = 100,
    .visible = true,
    .layerOrder = 1,
    .textures = {}
};

Sprite sprite2 = {
    .name = L"sprite2",
    .pos = {
        .x = (WINDOW_WIDTH - 95) / 2 + -79,
        .y = (WINDOW_HEIGHT - 100) / 2 - -27,
        .w = 95,
        .h = 100
    },
    .direction = 0,
    .costumeNumber = 0,
    .size = 100,
    .visible = true,
    .layerOrder = 1,
    .textures = {}
};

Sprite * const sprites[] = {
    &sprite2,
    &sprite,
    NULL
};


#endif

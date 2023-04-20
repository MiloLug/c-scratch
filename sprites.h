#ifndef SPRITES_COLLECTION_H
#define SPRITES_COLLECTION_H

#include "runtime/sprite.h"


Sprite sprite2 = {
    .name = "sprite2",
    .pos = {
        .x = (WINDOW_WIDTH - 95) / 2 + -130,
        .y = (WINDOW_HEIGHT - 100) / 2 - -67,
        .w = 95,
        .h = 100
    },
    .direction = 0,
    .costumeNumber = 0,
    .size = 100,
    .visible = true,
    .layerOrder = 1
};

Sprite sprite = {
    .name = "sprite",
    .pos = {
        .x = (WINDOW_WIDTH - 95) / 2 + 50,
        .y = (WINDOW_HEIGHT - 100) / 2 - 68,
        .w = 95,
        .h = 100
    },
    .direction = 0,
    .costumeNumber = 0,
    .size = 100,
    .visible = true,
    .layerOrder = 1
};

Sprite * const sprites[] = {
    &sprite2,
    &sprite,
    NULL
};


#endif

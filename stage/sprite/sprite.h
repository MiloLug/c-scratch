#include "sprites_common.h"


namespace NS_sprite {
    Sprite sprite({
        .name = L"Sprite",
        .safeName = L"sprite",
        .x = 100,
        .y = 0,
        .direction = 135,
        .costumeIndex = 1,
        .size = 100,
        .visible = false,
        .layerOrder = 1,
        .costumes = {
            {L"Costume 1", L"costume1.png", 0, 0, 95, 100},
        },
    });
}

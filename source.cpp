#include <stdio.h>
#include <thread>
#include <ctime>

#include "config.h"
#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/sprite_manager.h"
#include "scripts.h"


Sprite sprite2 = {
    .name = "sprite2",
    .pos = {
        .x = (WIDTH - 95) / 2 + -130,
        .y = (HEIGHT - 100) / 2 - -67,
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
        .x = (WIDTH - 95) / 2 + 50,
        .y = (HEIGHT - 100) / 2 - 68,
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


std::atomic<bool> shouldRun = true;

void sdl_loop(ScratchSDLWindow * window) {
    while (shouldRun) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                shouldRun = false;
        }

        renderSprites(*window, sprites);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization error\n");
        return 1;
    }

    ScratchSDLWindow window{"c scratch 0.1"};

    initSprites(window, sprites);
    std::thread sdl_loop_thread(sdl_loop, &window);

    auto sprite2Coro1 = sprite2Script1(&sprite2);
    auto sprite2Coro1_iter = sprite2Coro1.begin();
    auto const sprite2Coro1_end = sprite2Coro1.end();

    auto spriteCoro1 = spriteScript1(&sprite);
    auto spriteCoro1_iter = spriteCoro1.begin();
    auto const spriteCoro1_end = spriteCoro1.end();

    #ifndef ENABLE_TURBO
        const int clocks_per_frame = CLOCKS_PER_SEC / NON_TURBO_CALCULATION_FPS;
        std::clock_t previous_time = std::clock();
    #endif

    while (shouldRun) {
        #ifndef ENABLE_TURBO
            if (int(std::clock() - previous_time) < clocks_per_frame)
                continue;
            previous_time = std::clock();
        #endif

        if (sprite2Coro1_iter != sprite2Coro1_end) ++sprite2Coro1_iter;
        if (spriteCoro1_iter != spriteCoro1_end) ++spriteCoro1_iter;
    }
    
    sdl_loop_thread.join();

    return 0;
}

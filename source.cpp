#include <stdio.h>
#include <thread>
#include <ctime>

#include "config.h"
#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/sprite_manager.h"
#include "sprites.h"
#include "scripts.h"


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

    ScratchSDLWindow window{"c scratch 0.4"};

    initSprites(window, sprites);
    std::thread sdl_loop_thread(sdl_loop, &window);

    auto spriteCoro1 = spriteScript1(&sprite);
    auto spriteCoro1_iter = spriteCoro1.begin();
    auto const spriteCoro1_end = spriteCoro1.end();

    auto spriteCoro2 = spriteScript2(&sprite);
    auto spriteCoro2_iter = spriteCoro2.begin();
    auto const spriteCoro2_end = spriteCoro2.end();

    auto sprite2Coro1 = sprite2Script1(&sprite2);
    auto sprite2Coro1_iter = sprite2Coro1.begin();
    auto const sprite2Coro1_end = sprite2Coro1.end();

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

        if (spriteCoro1_iter != spriteCoro1_end) ++spriteCoro1_iter;
        if (spriteCoro2_iter != spriteCoro2_end) ++spriteCoro2_iter;
        if (sprite2Coro1_iter != sprite2Coro1_end) ++sprite2Coro1_iter;}
    
    sdl_loop_thread.join();

    return 0;
}

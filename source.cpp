#include <stdio.h>
#include <thread>

#define ENABLE_TURBO

#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/unique_generator.h"
#include "runtime/sprite_manager.h"


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


unique_generator<void*> sprite2Script1(Sprite * sprite) {
    co_yield NULL;
    // forever
    while (1) {
        if (keyState[SDL_SCANCODE_W]) {
            // change y by 1
            sprite->changeY(1);
        };
        
        if (keyState[SDL_SCANCODE_S]) {
            // change y by -1
            sprite->changeY(-1);
            
        };
        
        if (keyState[SDL_SCANCODE_D]) {
            // change x by 1
            sprite->changeX(1);
            
        };
        
        if (keyState[SDL_SCANCODE_A]) {
            // change x by -1
            sprite->changeX(-1);
            
        };
        
        co_yield NULL;
    };
    
};

unique_generator<void*> spriteScript1(Sprite * sprite) {
    co_yield NULL;
    // go to x: 0, y: 0
    sprite->goXY(0, 0);
    
    co_yield NULL;
    // forever
    while (1) {
        if (keyState[SDL_SCANCODE_UP]) {
            // change y by 1
            sprite->changeY(1);
        };
        
        if (keyState[SDL_SCANCODE_DOWN]) {
            // change y by -1
            sprite->changeY(-1);
            
        };
        
        if (keyState[SDL_SCANCODE_RIGHT]) {
            // change x by 1
            sprite->changeX(1);
            
        };
        
        if (keyState[SDL_SCANCODE_LEFT]) {
            // change x by -1
            sprite->changeX(-1);
            
        };
        
        co_yield NULL;
    };
    
};


std::atomic<bool> shouldRun = true;

void sdl_loop(ScratchSDLWindow * window) {
    while (shouldRun) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                shouldRun = false;
        }

        #ifndef ENABLE_TURBO
            setCanMove(sprites, true);
        #endif

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

    while (shouldRun) {
        if (sprite2Coro1_iter != sprite2Coro1_end) ++sprite2Coro1_iter;
        if (spriteCoro1_iter != spriteCoro1_end) ++spriteCoro1_iter;
    }
    
    sdl_loop_thread.join();

    return 0;
}

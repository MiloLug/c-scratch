
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cmath>
#include <coroutine>
#include <filesystem>
#include <string>
#include "unique_generator.h"
#include "scripts.h"
#include "motion.h"

#define WIDTH 480
#define HEIGHT 360


Sprite sprite = {
            .pos = {
                .x = (WIDTH - 95) / 2,
                .y = (HEIGHT - 100) / 2,
                .w = 95,
                .h = 100
            },
            .direction = 0,
            .costumeNumber = 0,
            .size = 100,
            .visible = 1,
            .layerOrder = 1,
            .textures = {}
};

int main(int argc, char* argv[]) {
    


    SDL_Window* window = SDL_CreateWindow("SDL", 100, 100, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    
    const std::filesystem::path spritepath{ "sprites/sprite/costumes" };

    for (auto const& dir_entry : std::filesystem::directory_iterator{ spritepath })
    {
        sprite.textures.push_back(IMG_LoadTexture(renderer, dir_entry.path().string().c_str()));
    };

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;

    auto v1 = coro1();
    auto v1_iter = v1.begin();
    auto const v1_end = v1.end();

    auto v2 = coro2();
    auto v2_iter = v2.begin();
    auto const v2_end = v2.end();

    bool proceed = true;

    bool shouldRun = true;

    while (shouldRun) {

        Sprite& current = sprite;
        if ((proceed |= v1_iter != v1_end)) ++v1_iter;
        if ((proceed |= v2_iter != v2_end)) ++v2_iter;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                shouldRun = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopyExF(renderer, sprite.textures[sprite.costumeNumber], NULL, &sprite.pos, sprite.direction, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

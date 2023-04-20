#ifndef RUNTIME_SDL_H
#define RUNTIME_SDL_H

#include "include_sdl.h"
#include <filesystem>
#include <atomic>


extern std::atomic<const Uint8 *> keyState;

class ScratchSDLWindow {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    ScratchSDLWindow(const char * name) {
        window = SDL_CreateWindow(name, 100, 100, WIDTH, HEIGHT, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    }

    ~ScratchSDLWindow() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
};


#endif

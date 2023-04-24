#ifndef RUNTIME_SDL_H
#define RUNTIME_SDL_H

#include <filesystem>
#include <atomic>
#include "include_sdl.h"
#include "../config.h"
#include <mutex>


extern const Uint8 * keyPressed;
extern std::mutex globalSyncMutex;

class ScratchSDLWindow {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    ScratchSDLWindow(const char * name) {
        window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

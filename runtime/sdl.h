#ifndef RUNTIME_SDL_H
#define RUNTIME_SDL_H

#include "config.h"

#include <iostream>
#include <thread>

#include "include_sdl.h"



extern const Uint8 * keyPressed;
extern std::mutex globalSyncMutex;

class ScratchSDLWindow {
private:
    void loop();
public:
    SDL_Window* window;
    SDL_Renderer* renderer;

    ScratchSDLWindow(const char * name);

    void updateFrameTiming(std::wostream& os = std::wcout, float period = 2.0f);
    std::thread runLoop();

    ~ScratchSDLWindow();
};


#endif

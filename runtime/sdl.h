#ifndef RUNTIME_SDL_H
#define RUNTIME_SDL_H

#include "config.h"

#include <iostream>
#include <thread>

#include "include_sdl.h"
#include "ts_synchronizer.h"


struct MouseState {
    int32_t x = 0;
    int32_t y = 0;
    bool isButtonDown = false;
};

extern const Uint8 * keyPressed;
extern MouseState volatile mouseState;
extern ThreadSafeSynchronizer screenUpdateLock;


class BlockWindowUpdates {
public:
    BlockWindowUpdates() { screenUpdateLock.take(); }
    ~BlockWindowUpdates() { screenUpdateLock.release(); }
};


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

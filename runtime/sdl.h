#ifndef RUNTIME_SDL_H
#define RUNTIME_SDL_H

#include "config.h"
#include "include_sdl.h"
#include "sdl_scancodes.h"
#include "ts_synchronizer.h"

#include <iostream>
#include <string>
#include <thread>


struct MouseState {
    int32_t x = 0;
    int32_t y = 0;
    bool isButtonDown = false;
};

extern const Uint8 * keyPressed;
extern MouseState volatile mouseState;
extern ThreadSafeSynchronizer screenUpdateLock;


static inline bool isKeyPressed(auto str) { return keyPressed[ScanCodesMap::getScanCode(str)]; }
static inline bool isKeyPressed(uint16_t scanCode) { return keyPressed[scanCode]; }


class BlockWindowUpdates {
public:
    BlockWindowUpdates() { screenUpdateLock.take(); }
    ~BlockWindowUpdates() { screenUpdateLock.release(); }
};


class ScratchSDLWindow {
private:
    void loop();

public:
    SDL_Window * window;
    SDL_Renderer * renderer;

    ScratchSDLWindow(const char * name);

    void updateFrameTiming(std::wostream & os = std::wcout, float period = 2.0f);
    std::thread runLoop();

    ~ScratchSDLWindow();
};


#endif

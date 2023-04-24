#include <stdio.h>
#include <thread>
#include <ctime>
#include <list>
#include <atomic>

#include "config.h"
#include "runtime/sdl.h"
#include "runtime/sprite_utils.h"
#include "sprites.h"
#include "scripts.h"


volatile bool shouldRun = true;

void sdlLoop(ScratchSDLWindow * window) {
    while (shouldRun) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_KEYDOWN:
                triggerCoroutines(ACTION_KEYDOWN|e.key.keysym.scancode, scriptBindings);
                break;
            case SDL_QUIT:
                shouldRun = false;
                break;
            default:
                break;
            }
        }

        renderSprites(*window, sprites);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        wprintf(L"SDL initialization error\n");
        return 1;
    }

    ScratchSDLWindow window{"c scratch 0.4"};

    initSprites(window, sprites);
    std::thread sdlLoopThread(sdlLoop, &window);

    std::list<Coroutine*> activeCoros;
    Coroutine * newCoroutine;

    triggerCoroutines(ACTION_START, scriptBindings);

    #ifndef ENABLE_TURBO
        const int clocks_per_frame = CLOCKS_PER_SEC / NON_TURBO_CALCULATION_FPS;
        std::clock_t previous_time = std::clock();
    #endif

    while (shouldRun) {
        #ifndef ENABLE_TURBO
            if (int(std::clock() - previous_time) < clocks_per_frame)
                continue;
            previous_time += clocks_per_frame;
        #endif

        while((newCoroutine = newActiveCoros.pop()) != NULL) {
            activeCoros.push_back(newCoroutine);
        }
        
        auto corosIter = activeCoros.begin();
        auto corosEnd = activeCoros.end();
        while(corosIter != corosEnd) {
            auto &coro = *corosIter;
            
            if(!coro->done()) {
                coro->resume();
                corosIter++;
            } else {
                delete coro;
                activeCoros.erase(corosIter++);
            }
        }
    }
    
    sdlLoopThread.join();

    return 0;
}

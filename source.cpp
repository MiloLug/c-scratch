#include <stdio.h>
#include <thread>
#include <ctime>
#include <list>
#include <atomic>
#include <iostream>

#include "config.h"
#include "runtime/sdl.h"
#include "runtime/sprite_utils.h"
#include "sprites.h"
#include "scripts.h"
#include "runtime/pen/pen.h"


void UpdateFrameTiming(std::wostream& os = std::wcout, float period = 2.0f)
{
    static unsigned int frames = 0;
    frames++;
    static auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    auto seconds = std::chrono::duration< float >( end - start ).count();
    if( seconds >= period )
    {
        os
            << frames << L" frames in "
            << std::setprecision( 1 ) << std::fixed << seconds << L" seconds | "
            << std::setprecision( 1 ) << std::fixed << frames / seconds << L" FPS ("
            << std::setprecision( 3 ) << std::fixed << seconds / frames * 1000.0 << L" ms/frame)\n";
        frames = 0;
        start = end;
    }
}

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

        SDL_RenderClear(window->renderer);

        if (Pen::hasChanges) {
            Pen::pixels.take();
            SDL_UpdateTexture((SDL_Texture *)Pen::texture, nullptr, (void *)Pen::pixelBuffer, Pen::canvasWidth*4);
            Pen::hasChanges = false;
            Pen::pixels.release();
        }
        SDL_RenderCopy(window->renderer, (SDL_Texture *)Pen::texture, nullptr, nullptr);
        renderSprites(*window, sprites);

        SDL_RenderPresent(window->renderer);

        #ifdef DEBUG
            UpdateFrameTiming();
        #endif
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        wprintf(L"SDL initialization error\n");
        return 1;
    }

    ScratchSDLWindow window{"c scratch 0.4"};
    
    initSprites(window, sprites);
    Pen::Initializer penInit(window);

    std::thread sdlLoopThread(sdlLoop, &window);

    std::list<Coroutine*> activeCoros;
    Coroutine * newCoroutine;

    triggerCoroutines(ACTION_START, scriptBindings);

    #ifndef ENABLE_TURBO
        const int clocks_per_frame = CLOCKS_PER_SEC / NON_TURBO_CALCULATION_FPS;
        std::clock_t previous_time = std::clock();
    #else
        #ifndef ENABLE_UNSAFE_NO_LOCKS
            int32_t unlockCounter = 0;
            Pen::pixels.take();
        #endif
    #endif

    while (shouldRun) {
        #ifndef ENABLE_TURBO
            if (int(std::clock() - previous_time) < clocks_per_frame)
                continue;
            previous_time += clocks_per_frame;
        #else
            #ifndef ENABLE_UNSAFE_NO_LOCKS
                unlockCounter++;
                if (unlockCounter == TURBO_LOCK_WINDOW_CYCLES) {
                    unlockCounter = 0;
                    Pen::pixels.release();
                    Pen::pixels.take();
                }
            #endif
        #endif
        

        while((newCoroutine = newActiveCoros.pop()) != NULL) {
            activeCoros.push_back(newCoroutine);
        }
        
        auto corosIter = activeCoros.begin();
        auto corosEnd = activeCoros.end();
        while(corosIter != corosEnd) {
            auto &coro = *corosIter;
            
            if(shouldRun = !coro->done()) {
                coro->resume();
                corosIter++;
            } else {
                delete coro;
                activeCoros.erase(corosIter++);
            }
        }
    }

    #ifdef ENABLE_TURBO
        #ifndef ENABLE_UNSAFE_NO_LOCKS
            Pen::pixels.release();
        #endif
    #endif
    
    sdlLoopThread.join();

    return 0;
}

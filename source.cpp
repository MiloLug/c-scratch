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
#include "runtime/pen.h"


volatile bool shouldRun = true;

void UpdateFrameTiming( std::wostream& os = std::wcout, float period = 2.0f )
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
            << std::setprecision( 1 ) << std::fixed << seconds << L" seconds = "
            << std::setprecision( 1 ) << std::fixed << frames / seconds << L" FPS ("
            << std::setprecision( 3 ) << std::fixed << seconds / frames * 1000.0 << L" ms/frame)\n";
        frames = 0;
        start = end;
    }
}

void sdlLoop(ScratchSDLWindow * window) {
    

    // const uint64_t texSize = WINDOW_WIDTH * WINDOW_HEIGHT * 4;
    // SDL_Texture* texture = SDL_CreateTexture(
    //     window->renderer,
    //     SDL_PIXELFORMAT_ARGB8888,
    //     SDL_TEXTUREACCESS_STREAMING, 
    //     WINDOW_WIDTH,
    //     WINDOW_HEIGHT
    // );

    // uint8_t pixels[texSize];
    // for (uint64_t i = 0; i < texSize; i++) pixels[i] = 0;
    
    while (shouldRun) {
        // for ( unsigned int i = 0; i < 1000; i++ ) {
        //     const unsigned int x = rand() % WINDOW_WIDTH;
        //     const unsigned int y = rand() % WINDOW_HEIGHT;

        //     const unsigned int offset = ( WINDOW_WIDTH * y * 4 ) + x * 4;
        //     pixels[ offset + 0 ] = rand() % 256;        // b
        //     pixels[ offset + 1 ] = rand() % 256;        // g
        //     pixels[ offset + 2 ] = rand() % 256;        // r
        //     pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
        // }

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

        // UpdateFrameTiming();
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
            
            if(shouldRun = !coro->done()) {
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

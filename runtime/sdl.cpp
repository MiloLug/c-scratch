#include "sdl.h"

#include "pen/pen.h"
#include "script/actions.h"
#include "script/script_manager.h"
#include "sprite/sprite_manager.h"
#include "time.h"

#include <iomanip>
#include <iostream>


const Uint8 * keyPressed = SDL_GetKeyboardState(nullptr);
MouseState volatile mouseState;
ThreadSafeSynchronizer screenUpdateLock;


ScratchSDLWindow::ScratchSDLWindow(const char * name) {
#ifdef SDL_USE_VULKAN
    window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_VULKAN
    );
#else
    window = SDL_CreateWindow(
        name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0
    );
#endif
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

void ScratchSDLWindow::updateFrameTiming(std::wostream & os, float period) {
    static unsigned int frames = 0;
    frames++;
    static auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    auto seconds = std::chrono::duration<float>(end - start).count();
    if (seconds >= period) {
        os << frames << L" frames in " << std::setprecision(1) << std::fixed << seconds
           << L" seconds | " << std::setprecision(1) << std::fixed << frames / seconds << L" FPS ("
           << std::setprecision(3) << std::fixed << seconds / frames * 1000.0 << L" ms/frame)\n";
        frames = 0;
        start = end;
    }
}

void ScratchSDLWindow::loop() {
    while (ScriptsShouldRun) {
        csTime.sync();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    ScriptManager::triggerScripts(ACTION_KEYDOWN | e.key.keysym.scancode);
                    break;
                case SDL_QUIT:
                    ScriptsShouldRun = false;
                    return;
                case SDL_MOUSEMOTION:
                    mouseState.x = e.motion.x - WINDOW_CENTER_X;
                    mouseState.y = WINDOW_CENTER_Y - e.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseState.isButtonDown = true;
                    SpriteManager::sendClickXY(mouseState.x, mouseState.y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseState.isButtonDown = false;
                    break;
                default:
                    break;
            }
        }

        // IT'S IMPORTANT
        // 1st lock - pixels
        // 2nd lock - screen

        bool pixelsTaken = false;
        if ((pixelsTaken = Pen::hasChanges)) {
            Pen::pixels.take();
        }

        if (!screenUpdateLock.is_blocked())
        {  // try to avoid redundant locks with double-check pattern
            screenUpdateLock.startProcessing();
            if (!screenUpdateLock.is_blocked()) {
                if (pixelsTaken) {
                    SDL_UpdateTexture(
                        (SDL_Texture *)Pen::texture,
                        NULL,
                        (void *)Pen::pixelBuffer,
                        Pen::canvasWidth * 4
                    );
                    Pen::hasChanges = false;
                    Pen::pixels.release();
                    pixelsTaken = false;
                }

                SDL_RenderClear(renderer);
                SpriteManager::renderBackdrop(renderer);
                SDL_RenderCopy(renderer, (SDL_Texture *)Pen::texture, NULL, NULL);
                SpriteManager::renderSprites(renderer);

                // better to release here since rendering takes too much time
                screenUpdateLock.stopProcessing();

                SDL_RenderPresent(renderer);
#ifdef DEBUG
                updateFrameTiming();
#endif
            } else {
                screenUpdateLock.stopProcessing();
            }
        }
        if (pixelsTaken) {
            Pen::pixels.release();
        }
    }
}

ScratchSDLWindow::~ScratchSDLWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

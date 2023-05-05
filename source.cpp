#include "config.h"
#include "runtime/debug.h"
#include "runtime/sdl.h"
#include "runtime/sprite_manager.h"
#include "runtime/script_manager.h"
#include "stage/sprites.h"
#include "stage/scripts.h"


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        wprintf(L"SDL initialization error\n");
        return 1;
    }

    ScratchSDLWindow window{APP_WINDOW_NAME};

    #ifdef DEBUG
        printDebugInfo(window);
    #endif
    
    SpriteManager::staticInit();
    ScriptManager::staticInit();

    Pen::Initializer penResource(window.renderer);
    SpriteManager::initSprites(window.renderer);
    SpriteManager::initBackdrop(window.renderer);

    auto sdlLoopThread = window.runLoop();
    ScriptManager::startScriptsLoop();
    sdlLoopThread.join();

    return 0;
}

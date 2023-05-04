#include "config.h"
#include "runtime/debug.h"
#include "runtime/sdl.h"
#include "runtime/sprite_manager.h"
#include "runtime/script_manager.h"
#include "scripts.h"
#include "sprites.h"


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        wprintf(L"SDL initialization error\n");
        return 1;
    }

    ScratchSDLWindow window{"c scratch 0.4"};

    #ifdef DEBUG
        printDebugInfo(window);
    #endif
    
    Pen::Initializer penResource(window.renderer);
    SpriteManager::initSprites(window.renderer, sprites);
    SpriteManager::initBackdrop(window.renderer, &backdrop);
    ScriptManager::bindScripts(scriptBindings);

    auto sdlLoopThread = window.runLoop();
    ScriptManager::startScriptsLoop();
    sdlLoopThread.join();

    return 0;
}

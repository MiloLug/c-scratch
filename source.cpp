#include "config.h"
#include "runtime/debug.h"
#include "runtime/sdl.h"
#include "runtime/sprite_utils.h"
#include "runtime/script_utils.h"
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
    
    Pen::Initializer penInit(window.renderer);
    initSprites(window.renderer, sprites);
    bindScripts(scriptBindings);

    auto sdlLoopThread = window.runLoop();
    startScriptsLoop();
    sdlLoopThread.join();

    return 0;
}

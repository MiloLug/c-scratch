#include "config.h"
#include "runtime/debug.h"
#include "runtime/script/script_manager.h"
#include "runtime/sdl.h"
#include "runtime/sprite/sprite_manager.h"
#include "stage/scripts.h"
#include "stage/sprites.h"
#include "runtime/thread.h"


int main(int argc, char * argv[]) {
    windowsTryShowConsole();

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

    auto scriptsThread = Thread(&ScriptManager::startScriptsLoop);
    window.loop();
    scriptsThread.join();

    return 0;
}

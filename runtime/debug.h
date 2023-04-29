#ifndef CSCRATCH_DEBUG_H
#define CSCRATCH_DEBUG_H

#include "sdl.h"


void printDebugInfo(ScratchSDLWindow &window) {
    #ifdef ENABLE_TURBO
        wprintf(L"using ENABLE_TURBO\n");
    #endif
    #ifdef ENABLE_UNSAFE_NO_LOCKS
        wprintf(L"using ENABLE_UNSAFE_NO_LOCKS\n");
    #endif
    #ifdef ENABLE_FAST_MATH
        wprintf(L"using ENABLE_FAST_MATH\n");
    #endif

    SDL_RendererInfo info = {0};
    if (SDL_GetRendererInfo(window.renderer, &info))
        wprintf(L"No current renderer info available\n");
    else
        wprintf(L"Current renderer: %s\n\n", info.name);

    uint8_t renderCount = SDL_GetNumRenderDrivers();
    wprintf(L"Render drivers available: %d\n", renderCount);

    for(uint8_t i = 0; i < renderCount; i++) {
        wprintf(L"Driver %d:\n", i + 1);

        if(!SDL_GetRenderDriverInfo(i, &info)) {
            wprintf(L"    Name: %s\n", info.name);
            wprintf(L"    Accelerated: %s\n", info.flags & SDL_RENDERER_ACCELERATED ? L"YES" : L"NO");
        } else {
            wprintf(L"    No driver info available\n");
        }
    }
    wprintf(L"\n");
}

#endif

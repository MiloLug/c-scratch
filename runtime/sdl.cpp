#include "sdl.h"


volatile bool shouldRun = true;
const Uint8 * keyPressed = SDL_GetKeyboardState(nullptr);
std::mutex globalSyncMutex;

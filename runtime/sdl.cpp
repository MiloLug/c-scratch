#include "sdl.h"


std::atomic<const Uint8 *> keyState = SDL_GetKeyboardState(nullptr);

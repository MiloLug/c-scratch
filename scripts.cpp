#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cmath>
#include <coroutine>
#include <filesystem>
#include <string>
#include "unique_generator.h"
#include "motion.h"
#include "scripts.h"

unique_generator<void*> coro1() {
    while (1) {
        changex(1);
        co_yield NULL;
    }
};

unique_generator<void*> coro2() {
    while (1) {
        const Uint8* keystate = SDL_GetKeyboardState(nullptr);

        if (keystate[SDL_SCANCODE_A]) {

            

        };

        co_yield NULL;
    };
};
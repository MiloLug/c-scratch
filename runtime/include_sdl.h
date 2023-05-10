#ifndef INCLUDE_SDL_H
#define INCLUDE_SDL_H

#include "config.h"

#if __has_include(<SDL.h>)
    #define SDL_HEADER(header) <header>
#else
    #define SDL_HEADER(header) <SDL2/header>
#endif

#include SDL_HEADER(SDL.h)
#include SDL_HEADER(SDL_image.h)
#include SDL_HEADER(SDL2_rotozoom.h)

#if __has_include(<vulkan/vulkan.h>)
    #define SDL_USE_VULKAN
#endif

constexpr float WINDOW_CENTER_X = WINDOW_WIDTH / 2.0f;
constexpr float WINDOW_CENTER_Y = WINDOW_HEIGHT / 2.0f;

#endif

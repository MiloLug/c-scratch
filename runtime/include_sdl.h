#ifndef INCLUDE_SDL_H
#define INCLUDE_SDL_H

#if __has_include (<SDL.h>)
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

#if __has_include (<SDL_image.h>)
    #include <SDL_image.h>
#else
    #include <SDL2/SDL_image.h>
#endif

#define WIDTH 480
#define HEIGHT 360

#endif

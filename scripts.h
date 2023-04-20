#include "runtime/unique_generator.h"
#include "runtime/sprite.h"
#include "runtime/sdl.h"


unique_generator<void*> sprite2Script1(Sprite * sprite) {
    co_yield NULL;
    // forever
    while (1) {
        if (keyState[SDL_SCANCODE_W]) {
            // change y by 1
            sprite->changeY(1);
        };
        
        if (keyState[SDL_SCANCODE_S]) {
            // change y by -1
            sprite->changeY(-1);
            
        };
        
        if (keyState[SDL_SCANCODE_D]) {
            // change x by 1
            sprite->changeX(1);
            
        };
        
        if (keyState[SDL_SCANCODE_A]) {
            // change x by -1
            sprite->changeX(-1);
            
        };
        
        co_yield NULL;
    };
    
};

unique_generator<void*> spriteScript1(Sprite * sprite) {
    co_yield NULL;
    // go to x: 0, y: 0
    sprite->goXY(0, 0);
    
    co_yield NULL;
    // forever
    while (1) {
        if (keyState[SDL_SCANCODE_UP]) {
            // change y by 1
            sprite->changeY(1);
        };
        
        if (keyState[SDL_SCANCODE_DOWN]) {
            // change y by -1
            sprite->changeY(-1);
            
        };
        
        if (keyState[SDL_SCANCODE_RIGHT]) {
            // change x by 1
            sprite->changeX(1);
            
        };
        
        if (keyState[SDL_SCANCODE_LEFT]) {
            // change x by -1
            sprite->changeX(-1);
            
        };
        
        co_yield NULL;
    };
    
};

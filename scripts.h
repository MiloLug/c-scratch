#include "runtime/unique_generator.h"
#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/array.h"
#include "runtime/math.h"


unique_generator<void*> spriteScript1(Sprite * sprite) {
    co_yield NULL;
    // go to x: 50, y: 0
    sprite->goXY(50, 0);
    
    // point in direction 90
    sprite->point(90);
    
    // go to x: (sprite->x * 2), y: 0
    sprite->goXY((sprite->x * 2), 0);
    
    // point in direction (sprite->getDirection() * 1.5)
    sprite->point((sprite->getDirection() * 1.5));
    
};

unique_generator<void *> testCoro(Sprite * sprite) { 
    ValueArray arr1;
    const double rad90 = 90.0 * (MPI / 180.0);

    for (int i = 0; i < 10000000; i++) {
        arr1.push(10);
        co_yield NULL;
    }

    for (int t = 0; t < 10; t++) {
        for (int i = 0; i < arr1.length; i++) {
            arr1.set(i, arr1.get(i)->number + sin(rad90 * (double)rand() / RAND_MAX));
            co_yield NULL;
        }
        co_yield NULL;
    }
}

unique_generator<void*> spriteScript2(Sprite * sprite) {
    co_yield NULL;
    // forever
    while (1) {
        if (keyPressed[SDL_SCANCODE_UP]) {
            // move 1 steps
            sprite->move(1);
            
        };
        
        if (keyPressed[SDL_SCANCODE_DOWN]) {
            // move -1 steps
            sprite->move(-1);
            
        };
        
        if (keyPressed[SDL_SCANCODE_RIGHT]) {
            // turn right 1 degrees
            sprite->turnRight(1);
            
        };
        
        if (keyPressed[SDL_SCANCODE_LEFT]) {
            // turn left 1 degrees
            sprite->turnLeft(1);
            
        };
        
        co_yield NULL;
    };
    
};

unique_generator<void*> sprite2Script1(Sprite * sprite) {
    co_yield NULL;
    // forever
    while (1) {
        if (keyPressed[SDL_SCANCODE_W]) {
            // change y by 1
            sprite->changeY(1);
            
        };
        
        if (keyPressed[SDL_SCANCODE_S]) {
            // change y by -1
            sprite->changeY(-1);
            
        };
        
        if (keyPressed[SDL_SCANCODE_D]) {
            // change x by 1
            sprite->changeX(1);
            
        };
        
        if (keyPressed[SDL_SCANCODE_A]) {
            // change x by -1
            sprite->changeX(-1);
            
        };
        
        co_yield NULL;
    };
    
};

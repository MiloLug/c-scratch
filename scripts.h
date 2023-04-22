#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/array.h"
#include "runtime/math.h"
#include "runtime/coroutines.h"


Coroutine spriteScript1(Sprite * sprite) {
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

Coroutine test1(Sprite * sprite) {
    Value x = sprite->x;
    Value y = sprite->y;
    Value angle = 0;

    int i = 100000000;
    while(i--) {
        angle = fmod(angle + 0.00001, 360.0);
        sprite->goXY(
            30.0 * mSin(angle) + x,
            30.0 * mCos(angle) + y
        );
        co_yield NULL;
    }
}

Coroutine testCoro(Sprite * sprite) {
    co_yield test1(sprite);
    wprintf(L"asdsadsadsad\n");
    co_yield NULL;
    // wprintf(L"A 1\n");
    // ValueArray arr1;

    // for (Value i = 0; i < 10000000; i++) {
    //     arr1.push(10);
    //     co_yield NULL;
    // }
    // wprintf(L"A 2\n");

    // for (Value t = 0; t < 10; t+=1) {
    //     for (Value i = 0; i < arr1.length; i++) {
    //         arr1.set(i, *arr1.get(i) + mSin(mRandInRange(0, 360)));
    //         co_yield NULL;
    //     }
    //     co_yield NULL;
    // }
    // wprintf(L"A 3\n");
}

Coroutine spriteScript2(Sprite * sprite) {
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

Coroutine sprite2Script1(Sprite * sprite) {
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

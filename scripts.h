#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/array.h"
#include "runtime/math.h"
#include "runtime/coroutines.h"
#include "runtime/string_utils.h"
#include "runtime/script_utils.h"
#include "runtime/pen.h"
#include "sprites.h"


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
            30.0 * degSin(angle) + x,
            30.0 * degCos(angle) + y
        );
        co_yield NULL;
    }
}

Value angle = 0;

Coroutine testKeyPressR(Sprite * sprite) {
    co_yield NULL;
    sprite->goXY(
        sprite->x - 30.0 * degSin(angle),
        sprite->y - 30.0 * degCos(angle)
    );

    angle = fmod(angle + 2, 360.0);
    sprite->goXY(
        sprite->x + 30.0 * degSin(angle),
        sprite->y + 30.0 * degCos(angle)
    );
}

ValueArray testGlobArr({1, 2, 3, L"GG", L"test str", 20903.3298741239283});

Value div_ = 700;

Coroutine testCoro(Sprite * sprite) {
    // testGlobArr.push(L"asdsad");
    // co_yield test1(sprite);
    // wprintf(L"asdsadsadsad\n");
    co_yield NULL;
    // wprintf(L"A 1\n");
    // ValueArray arr1;

    // for (Value i = 0; i < 10000000; i++) {
    //     arr1.push(10);
    //     co_yield NULL;
    // }
    // wprintf(L"A 2\n");
    
    // for (Value t = 0; t < 10; t+=1) {
    //     for (Value i = 1; i <= arr1.length; i++) {
    //         arr1.set(i, arr1.get(i) + degSin(randInRange(0, 360)));
    //         co_yield NULL;
    //     }
    //     co_yield NULL;
    // }
    // wprintf(L"A 3\n");

    // Value max = (double)0xFFFFFFFF;

    // Value off = 0;
    // Value t = 1;

    // while(1) {
    //     off += t;
    //     if (off > 30) {
    //         t = -1;
    //     } else if (off < 1) {
    //         t = 1;
    //     }
    //     for (Value x = 0; x < WINDOW_WIDTH; x++) {
    //         for (Value y = 0; y < WINDOW_HEIGHT; y++) {
    //             // max--;
    //             Pen::drawLine(x, y, x, y, 1, (uint64_t)((double)0xFF0FF0FF * (
    //                 // 1 - (sqrt(pow(sprite->pos.x + sprite->pos.w / 2.0 - x, 2.0) + pow(sprite->pos.y + sprite->pos.h / 2.0 - y, 2.0)) + off) / (radSin(div_) || radCos(div_))
    //                 1 - radSin(2 - (sqrt(pow(sprite->pos.x + sprite->pos.w / 2.0 - x, 2.0) + pow(sprite->pos.y + sprite->pos.h / 2.0 - y, 2.0)) + off) / div_)
    //             )));
    //             co_yield NULL;
    //         }
    //         co_yield NULL;
    //     }
    //     co_yield NULL;
    // }

    // sprite->point(121);
    // sprite->goXY(-20, -30);
    

    // for (Value i = 0; i < 1000000; i++) {
        // wprintf(L"PPP %f\n", fmod(i / 100, 360.0));
    // auto tmp = rotozoomSurface(sprite->getCostumeSurface(), -sprite->direction, 1, 1);
    // Pen::stamp((WINDOW_WIDTH - tmp->clip_rect.w) / 2.0 + sprite->x + 0.5, (WINDOW_HEIGHT - tmp->h) / 2.0 - sprite->y + 0.5, tmp);
    // SDL_FreeSurface(tmp);
    // co_yield NULL;
    // }

    // Pen::drawLine(-2, -20, 100, 140, 11, 0xFF0F7F4F);

    // Pen::drawLine(100, 160, 100, 160, 12, 0xFF0F7F4F);

    for(Value i = 0; i < 1; i++) {
        Pen::drawLine(100, 200, 100, 200, 35, 0xFF0F7F4F);
        co_yield NULL;
    }

    while(1) co_yield NULL;
    // for(Value i = 0; i < 1000; i++) {
    //     for(Value x = 0; x < WINDOW_WIDTH; x++) {
    //         for(Value y = 0; y < WINDOW_HEIGHT; y++) {
    //             Pen::drawLine(x, y, x, y, 1, 0x0100FF00);
    //             co_yield NULL;
    //         }
    //         co_yield NULL;
    //     }
    //     co_yield NULL;
    // }

    co_yield NULL;

    // VARIABLES EXAMPLE
    // Value gg = 1;

    // for (Value i = 0; i < 10000; i++) {
    //     Value kek = gg + 15;
    //     Value bruh = kek.toString();
    //     Value test = bruh == (gg + 15);
    //     ValueArray testArr({1, 2, L"gg 10"});

    //     wprintf(L"lol1: %ls\n", test.toString());

    //     test = join(join(join(join(gg, L" + "), 15), L" = "), bruh);
    //     wprintf(L"lol2: %i\n", join(join(join(join(gg, L" + "), 15), L" = "), bruh) == L"333 + 15 = 348");
    //     wprintf(L"lol3: %ls\n", test.toString());

    //     test = 20 + join(1, 50);
    //     wprintf(L"lol4: %ls\n", test.toString());

    //     test = letterOf(test, 2) + 20;
    //     wprintf(L"lol5: %ls\n", test.toString());

    //     test = testArr;
    //     testArr.push(testArr);
    //     test = join(L"testArr = ", testArr);
    //     wprintf(L"lol7: %ls\n", test.toString());


    //     wprintf(L"\n== I: %ls ==\n", i.toString());

    //     co_yield NULL;
    // }
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

Coroutine moveY1(Sprite * sprite) {
    co_yield NULL;
    sprite->changeY(10);
}

Coroutine moveYN1(Sprite * sprite) {
    co_yield NULL;
    sprite->changeY(-10);
}

Coroutine moveX1(Sprite * sprite) {
    co_yield NULL;
    sprite->changeX(10);
}

Coroutine moveXN1(Sprite * sprite) {
    co_yield NULL;
    sprite->changeX(-10);
}

Coroutine incDiv(Sprite * sprite) {
    co_yield NULL;
    div_ += 1;
}
Coroutine decDiv(Sprite * sprite) {
    co_yield NULL;
    div_ += -1;
    if (div_ <= 0) div_ = 1;
}


BindingsMap scriptBindings = {
    {ACTION_START, {
        {&sprite, {
            // spriteScript1,
            // spriteScript2
        }},
        {&sprite2, {
            testCoro,
            // sprite2Script1
        }}
    }},
    // {ACTION_KEYDOWN|SDL_SCANCODE_W, {
    //     {&sprite2, {
    //         moveY1
    //     }}
    // }},
    // {ACTION_KEYDOWN|SDL_SCANCODE_S, {
    //     {&sprite2, {
    //         moveYN1
    //     }}
    // }},
    // {ACTION_KEYDOWN|SDL_SCANCODE_D, {
    //     {&sprite2, {
    //         moveX1
    //     }}
    // }},
    // {ACTION_KEYDOWN|SDL_SCANCODE_A, {
    //     {&sprite2, {
    //         moveXN1
    //     }}
    // }},
    // {ACTION_KEYDOWN|SDL_SCANCODE_Q, {
    //     {&sprite2, {
    //         incDiv
    //     }}
    // }},
    // {ACTION_KEYDOWN|SDL_SCANCODE_E, {
    //     {&sprite2, {
    //         decDiv
    //     }}
    // }}
};

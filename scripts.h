#ifndef CSCRATCH_SCRIPTS_H
#define CSCRATCH_SCRIPTS_H

#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/array.h"
#include "runtime/math.h"
#include "runtime/coroutines.h"
#include "runtime/string_utils.h"
#include "runtime/script_utils.h"
#include "runtime/pen/pen.h"
#include "runtime/control_flow.h"
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

Coroutine kek221(Sprite * sprite) {
    wprintf(L"I 1\n");
    co_return;
}
Coroutine kek222(Sprite * sprite) {
    wprintf(L"I 2\n");
    co_return;
}
Coroutine kek223(Sprite * sprite) {
    wprintf(L"I 3\n");
    co_return;
}
Coroutine kek224(Sprite * sprite) {
    wprintf(L"I 4\n");
    co_return;
}

Coroutine kek22(Sprite * sprite) {
    wprintf(L"I\n");
    wait(kek221(sprite));
    wait(kek222(sprite));
    wait(kek223(sprite));
    wait(kek224(sprite));
    co_return;
}

Coroutine testCoro(Sprite * sprite) {
    // testGlobArr.push(L"asdsad");
    // co_yield test1(sprite);
    // wprintf(L"asdsadsadsad\n");
    // wprintf(L"A\n");
    wait(kek22(sprite));

    // // auto __test = kek22(sprite);
    // // if (__coroNext(__test))
    // //     co_yield __test;

    // wprintf(L"B\n");
    wprintf(L"A 1\n");
    ValueArray arr1;

    // repeat (10000000) {
    //     arr1.push(10);
    //     yield;
    // }

    wprintf(L"A 2\n");
    
    // repeat (10) {
    //     for (Value i = 1; i <= arr1.length; i++) {
    //         arr1.set(i, arr1.get(i) + degSin(randInRange(0, 360)));
    //         yield;
    //     }
    //     yield;
    // }
    wprintf(L"A 3\n");

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
    // sprite->goXY(500, 500);
    

    

    
    // Pen::drawLine(-2, -20, 100, 140, 11, 0xFF0F7F4F);

    // Pen::drawLine(100, 160, 100, 160, 12, 0xFF0F7F4F);

    // for(Value i = 0; i < 100; i++) {
    //     // Pen::drawLine(100, 100, 100, 100, 10, 0x4F0F7F4F);
    //     // Pen::drawLine(100, 120, 100, 120, 12, 0x4F0F7F4F);
    //     // Pen::drawLine(100, 140, 100, 140, 14, 0x4F0F7F4F);
    //     // Pen::drawLine(100, 160, 100, 160, 16, 0x4F0F7F4F);
    //     // Pen::drawLine(100, 180, 100, 180, 18, 0x4F0F7F4F);

    //     // Pen::drawLine(140, 100, 140, 100, 20, 0x4F0F7F4F);
    //     // Pen::drawLine(140, 130, 140, 130, 22, 0x4F0F7F4F);
    //     // Pen::drawLine(140, 160, 140, 160, 24, 0x4F0F7F4F);
    //     // Pen::drawLine(140, 190, 140, 190, 26, 0x4F0F7F4F);
    //     // Pen::drawLine(140, 220, 140, 220, 28, 0x4F0F7F4F);

    //     // Pen::drawLine(180, 100, 180, 100, 30, 0x4F0F7F4F);
    //     // Pen::drawLine(180, 140, 180, 140, 32, 0x4F0F7F4F);
    //     // Pen::drawLine(180, 180, 180, 180, 34, 0x4F0F7F4F);
    //     Pen_safe(Pen::drawLine(180, 220, 180, 220, 36, 0x4F0F7F4F));
        
    //     co_yield NULL;
    // }

    // double seed = 546564;
    // OpenSimplexNoise::Noise noise(546564);
    // uint8_t octaves = 4;

    // auto gradient = IMG_Load("sprites/gradient.png");
    // if (gradient->format->format != SDL_PIXELFORMAT_ARGB8888) {
    //     auto tmp = SDL_ConvertSurfaceFormat(gradient, SDL_PIXELFORMAT_ARGB8888, 0);
    //     SDL_FreeSurface(gradient);
    //     gradient = tmp;
    // }

    // const auto gradW = gradient->clip_rect.w;
    // const auto gradPixels = (uint32_t *)gradient->pixels;
    // double scale = 1;
    // int64_t
    //     offsetX = 200,
    //     offsetY = 10;

    
    // while(1) {
        
    //     if (keyPressed[SDL_SCANCODE_KP_PLUS]) {
    //         scale -= 0.01 * scale * 5;
    //     }
    //     if (keyPressed[SDL_SCANCODE_KP_MINUS]) {
    //         scale += 0.01 * scale * 5;
    //     }
    //     if (keyPressed[SDL_SCANCODE_UP]) {
    //         offsetY -= 10 / scale * scale;
    //     }
    //     if (keyPressed[SDL_SCANCODE_DOWN]) {
    //         offsetY += 10 / scale * scale;
    //     }
    //     if (keyPressed[SDL_SCANCODE_LEFT]) {
    //         offsetX -= 10 / scale * scale;
    //     }
    //     if (keyPressed[SDL_SCANCODE_RIGHT]) {
    //         offsetX += 10 / scale * scale;
    //     }

    //     if (scale < 0.0001) scale = 0.0001;

    //     // wprintf(L"ASDasdsadsadsad\n");

    //     // Pen::eraseAll();
    //     Pen::pixels.take();
    //     for (int32_t x = 0; x < WINDOW_WIDTH; x++) {
    //         for (int32_t y = 0; y < WINDOW_HEIGHT; y++) {
    //             auto scaledX = double(x + offsetX) * scale;
    //             auto scaledY = double(y + offsetY) * scale;

    //             double
    //                 res = 0.,
    //                 amp = .5,
    //                 freq = 1.95,
    //                 _x = scaledX,
    //                 _y = scaledY;

    //             for (auto i = 0; i < octaves; i++) {
    //                 res += amp * noise.eval(_x, _y);
    //                 amp *= .5;
    //                 _x *= freq;
    //                 _y *= freq;
    //             }

    //             res = ((res + 1.) / 2.);

    //             Pen::drawPixel(
    //                 x,
    //                 y,
    //                 res == 1
    //                     ? gradPixels[gradW - 1]
    //                     : gradPixels[(uint64_t)(res * gradW)]
    //             );
    //         }
    //     }
    //     Pen::hasChanges = true;
    //     Pen::pixels.release();

    //     co_yield NULL;
    // }
    
    
    // sprite->goXY(-(WINDOW_WIDTH / 2.0f), (WINDOW_HEIGHT / 2.0f));
    // sprite->penDown();
    
    // repeat (1000) {
    //     for(Value x = 0; x < WINDOW_WIDTH; x++) {
    //         for(Value y = 0; y < WINDOW_HEIGHT; y++) {
    //             sprite->changeY(-1);
    //             co_yield NULL;
    //         }
    //         sprite->penUp();
    //         sprite->changeX(1);
    //         sprite->changeY(WINDOW_HEIGHT);
    //         sprite->penDown();
    //         co_yield NULL;
    //     }
    //     co_yield NULL;
    // }

    // Pen::drawLine(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_HEIGHT - 5, 0xFF0FFFF0);
    // while(1) co_yield NULL;
    // co_yield NULL;

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
    co_return;
}

Coroutine spriteScript2(Sprite * sprite) {
    co_yield NULL;

    sprite->penDown();
    // forever
    forever {
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
    forever {
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

Coroutine makeStamp(Sprite * sprite) {
    co_yield NULL;

    sprite->penStamp();
}


const BindingsMap scriptBindings = {
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
    // {ACTION_KEYDOWN|SDL_SCANCODE_P, {
    //     {&sprite, {
    //         makeStamp
    //     }}
    // }},
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


#endif

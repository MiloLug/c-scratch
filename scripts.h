#ifndef CSCRATCH_SCRIPTS_H
#define CSCRATCH_SCRIPTS_H

#include "runtime/sprite.h"
#include "runtime/sdl.h"
#include "runtime/array.h"
#include "runtime/math.h"
#include "runtime/coroutines.h"
#include "runtime/string_utils.h"
#include "runtime/script_manager.h"
#include "runtime/pen/pen.h"
#include "runtime/control_flow.h"
#include "runtime/utils.h"
#include "sprites.h"


Value v_size_100_2 = 90000;
Value v_ACCUMULATED_EXPOSURE = 0.0005;
Value v_colorValue = 10.526212;
Value v_dataIndex = 261072;
Value v_down = 90300;
Value v_force = 0;
Value v_frame = 880;
Value v_i = 90000;
Value v_j = 300;
Value v_light_position = 18060;
Value v_light_size = 50;
Value v_math_min = 0.203173;
Value v_sin = 5.100006;
Value v_size = 3;
Value v_size_100 = 300;
Value v_size_2 = 9;
Value v_speed = 1;
Value v_tick = 88;
Value v_timer = 30.396;
Value v_up = 89700;

ValueArray l_accumulatedLight;
ValueArray l_accumulatedLight_blue;
ValueArray l_accumulatedLight_green;
ValueArray l_accumulatedLight_red;
ValueArray l_COLOR_SHIFT;
ValueArray l_GLASS_COLORS;
ValueArray l_imgData_blue;
ValueArray l_imgData_green;
ValueArray l_imgData_red;
ValueArray l_pixelMass;
ValueArray l_waveHeight;
ValueArray l_waveHeight_blue;
ValueArray l_waveHeight_green;
ValueArray l_waveHeight_red;
ValueArray l_waveVelocity;
ValueArray l_waveVelocity_blue;
ValueArray l_waveVelocity_green;
ValueArray l_waveVelocity_red;


class StageSprites {
public:
    template<typename T1, typename T2>
    static Coroutine spriteProcedure_math_min(Sprite * sprite, T1 &&arg_1, T2 &&arg_2) {
        if (arg_1 > arg_2) {
            v_math_min = arg_2;
        } else {
            v_math_min = arg_1;
        }

        stopThisScript();
    }

    static Coroutine spriteProcedure_loop_unrolling_1(Sprite * sprite) {
        v_i += 1.0;
        v_up += 1.0;
        v_down += 1.0;
        v_speed = l_pixelMass.get(v_i);

        l_waveVelocity_red.set(
            v_i,
            l_waveVelocity_red.get(v_i) + (
                (
                    l_waveHeight_red.get(v_i - 1.0)
                    + l_waveHeight_red.get(v_i + 1.0)
                    + l_waveHeight_red.get(v_up)
                    + l_waveHeight_red.get(v_down)
                ) / 4.0
                - l_waveHeight_red.get(v_i)
            )
            * (v_speed - l_COLOR_SHIFT.get(1.0))
        );
        l_waveVelocity_green.set(
            v_i,
            l_waveVelocity_green.get(v_i) + (
                (
                    l_waveHeight_green.get(v_i - 1.0)
                    + l_waveHeight_green.get(v_i + 1.0)
                    + l_waveHeight_green.get(v_up)
                    + l_waveHeight_green.get(v_down)
                ) / 4.0
                - l_waveHeight_green.get(v_i)
            )
            * (v_speed - l_COLOR_SHIFT.get(2.0))
        );
        l_waveVelocity_blue.set(
            v_i,
            l_waveVelocity_blue.get(v_i) + (
                (
                    l_waveHeight_blue.get(v_i - 1.0)
                    + l_waveHeight_blue.get(v_i + 1.0)
                    + l_waveHeight_blue.get(v_up)
                    + l_waveHeight_blue.get(v_down)
                ) / 4.0
                - l_waveHeight_blue.get(v_i)
            )
            * (v_speed - l_COLOR_SHIFT.get(3.0))
        );

        stopThisScript();
    }

    static Coroutine spriteProcedure_loop_unrolling_10(Sprite * sprite) {
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1(sprite));

        stopThisScript();
    }

    static Coroutine spriteProcedure_loop_unrolling_100(Sprite * sprite) {
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        cs_wait(spriteProcedure_loop_unrolling_10(sprite));
        
        stopThisScript();
    }

    static Coroutine spriteProcedure_loop_unrolling_1000(Sprite * sprite) {
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));
        cs_wait(spriteProcedure_loop_unrolling_100(sprite));

        stopThisScript();
    }

    static Coroutine spriteProcedure_loop_unrolling_10000(Sprite * sprite) {
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));
        cs_wait(spriteProcedure_loop_unrolling_1000(sprite));

        stopThisScript();
    }

    static Coroutine spriteProcedure_calculate(Sprite * sprite) {
        if (v_frame <= 300.0) {
            v_i = v_light_position;
            v_sin = degSin(fmod(v_frame * 45.83662439235437 - 90.0, 360.0)) * 12.0;

            repeat (v_light_size) {
                v_i += 1.0;

                l_waveHeight_red.set(v_i, v_sin);
                l_waveHeight_green.set(v_i, v_sin);
                l_waveHeight_blue.set(v_i, v_sin);

                cs_yield;
            }
        }

        v_i = 0;

        repeat (v_size_100_2) {
            v_i += 1.0;

            l_waveHeight_red.set(v_i, l_waveHeight_red.get(v_i) + l_waveVelocity_red.get(v_i));
            l_waveHeight_green.set(v_i, l_waveHeight_green.get(v_i) + l_waveVelocity_green.get(v_i));
            l_waveHeight_blue.set(v_i, l_waveHeight_blue.get(v_i) + l_waveVelocity_blue.get(v_i));
            l_accumulatedLight_red.set(v_i, l_accumulatedLight_red.get(v_i) + abs(l_waveHeight_red.get(v_i)) * v_ACCUMULATED_EXPOSURE);
            l_accumulatedLight_green.set(v_i, l_accumulatedLight_green.get(v_i) + abs(l_waveHeight_green.get(v_i)) * v_ACCUMULATED_EXPOSURE);
            l_accumulatedLight_blue.set(v_i, l_accumulatedLight_blue.get(v_i) + abs(l_waveHeight_blue.get(v_i)) * v_ACCUMULATED_EXPOSURE);

            cs_yield;
        }

        v_i = 0;
        v_up = v_size_100 * -1.0;
        v_down = v_size_100;

        repeat (v_size_2) {
            cs_wait(spriteProcedure_loop_unrolling_10000(sprite));

            cs_yield;
        }

        v_frame += 1.0;

        stopThisScript();
    }

    static Coroutine spriteProcedure_render(Sprite * sprite) {
        BlockWindowUpdates blocker;
        
        Pen_safe(Pen::eraseAll());
        sprite->goXY(v_size_100 / 2.0 * -1.0, v_size_100 / 2.0);

        v_i = 0;

        repeat (v_size_100) {
            sprite->setY(v_size_100 / 2.0);
            sprite->penDown();

            repeat (v_size_100) {
                v_i += 1.0;

                cs_wait(spriteProcedure_math_min(sprite, l_accumulatedLight_red.get(v_i), 1.0));
                v_colorValue = v_math_min * v_math_min * 255.0;

                if (l_pixelMass.get(v_i) < 1.0) {
                    cs_wait(spriteProcedure_math_min(sprite, v_colorValue + l_GLASS_COLORS.get(1.0), 255.0));
                    v_colorValue = v_math_min;
                }
                l_imgData_red.set(v_i, v_colorValue);

                cs_wait(spriteProcedure_math_min(sprite, l_accumulatedLight_green.get(v_i), 1.0));
                v_colorValue = v_math_min * v_math_min * 255.0;

                if (l_pixelMass.get(v_i) < 1.0) {
                    cs_wait(spriteProcedure_math_min(sprite, v_colorValue + l_GLASS_COLORS.get(2.0), 255.0));
                    v_colorValue = v_math_min;
                }
                l_imgData_green.set(v_i, v_colorValue);

                cs_wait(spriteProcedure_math_min(sprite, l_accumulatedLight_blue.get(v_i), 1.0));
                v_colorValue = v_math_min * v_math_min * 255.0;

                if (l_pixelMass.get(v_i) < 1.0) {
                    cs_wait(spriteProcedure_math_min(sprite, v_colorValue + l_GLASS_COLORS.get(3.0), 255.0));
                    v_colorValue = v_math_min;
                }
                l_imgData_blue.set(v_i, v_colorValue);

                sprite->penSetColor(round(l_imgData_red.get(v_i)) * 65536.0 + round(l_imgData_green.get(v_i)) * 256.0 + round(l_imgData_blue.get(v_i)));
                sprite->changeY(-1);

                cs_yield;
            }

            sprite->penUp();
            sprite->changeX(1);   

            cs_yield;
        }

        stopThisScript();
    }

    static Coroutine spriteStartScript1(Sprite * sprite) {
        l_imgData_blue.clean();
        l_imgData_green.clean();
        l_imgData_red.clean();
        l_waveHeight_red.clean();
        l_waveHeight_green.clean();
        l_waveHeight_blue.clean();
        l_waveVelocity_red.clean();
        l_waveVelocity_green.clean();
        l_waveVelocity_blue.clean();
        l_accumulatedLight_red.clean();
        l_accumulatedLight_green.clean();
        l_accumulatedLight_blue.clean();
        l_pixelMass.clean();

        l_GLASS_COLORS.clean();
        l_GLASS_COLORS.push(50.0);
        l_GLASS_COLORS.push(60.0);
        l_GLASS_COLORS.push(70.0);

        l_COLOR_SHIFT.clean();
        l_COLOR_SHIFT.push(0.06);
        l_COLOR_SHIFT.push(0);
        l_COLOR_SHIFT.push(-0.06);

        v_ACCUMULATED_EXPOSURE = 0.0005;
        v_frame = 0;
        v_i = 0;
        v_tick = 0;
        v_size_2 = v_size * v_size;
        v_size_100 = v_size * 100.0;
        v_size_100_2 = v_size_100 * v_size_100;

        v_light_size = v_size_100 / 6.0;
        v_light_position = floor(v_size_100 / 5.0) * v_size_100 + floor(v_size_100 / 5.0);

        wprintf(L"1.0 - l_COLOR_SHIFT.get(3.0) = %f \n", 1.0 - l_COLOR_SHIFT.get(3.0));

        repeat (v_size_100) {
            v_j = 0;

            repeat (v_size_100) {
                l_waveHeight_red.push(0);
                l_waveHeight_green.push(0);
                l_waveHeight_blue.push(0);
                l_waveVelocity_red.push(0);
                l_waveVelocity_green.push(0);
                l_waveVelocity_blue.push(0);
                l_accumulatedLight_red.push(0);
                l_accumulatedLight_green.push(0);
                l_accumulatedLight_blue.push(0);
                l_imgData_red.push(0);
                l_imgData_green.push(0);
                l_imgData_blue.push(0);
                l_pixelMass.push(1.0);

                if (sqrt((v_i - v_size_100 / 2.0) * (v_i - v_size_100 / 2.0) + (v_j - v_size_100 / 2.0) * (v_j - v_size_100 / 2.0)) < (v_size_100 / 4.0)) {
                    l_pixelMass.set(v_i * v_size_100 + v_j + 1.0, 3.0 / 4.0);
                }

                v_j += 1.0;

                cs_yield;
            }

            v_i += 1.0;

            cs_yield;
        }

        forever {
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_calculate(sprite));
            cs_wait(spriteProcedure_render(sprite));
            v_tick += 1;

            // wprintf(L"tick = %ls\n", v_tick.toString());

            cs_yield;
        }

        stopThisScript();
    }

    static Coroutine spriteStartScript2(Sprite * sprite) {
        forever {
            waitUntil(v_tick == 100);
            v_timer = timer;
            v_tick = 0;

            // don't have graphical output rn, so it's just like this
            wprintf(L"timer = %ls\n", v_timer.toString());

            cs_yield;
        }
        
        stopThisScript();
    }
};


const ScriptManager::BindingsMap scriptBindings = {
    {ACTION_START, {
        {&sprite, {
            StageSprites::spriteStartScript1,
            StageSprites::spriteStartScript2,
            // testCoro,
        }},
    }},
};


#endif

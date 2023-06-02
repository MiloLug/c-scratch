#include "scripts_common.h"


namespace NS_sprite {
    Var v_size_100_2 = 90000;
    Var v_ACCUMULATED_EXPOSURE = 0.0005;
    Var v_colorValue = 10.526212;
    Var v_dataIndex = 261072;
    Var v_down = 90300;
    Var v_force = 0;
    Var v_frame = 880;
    Var v_i = 90000;
    Var v_j = 300;
    Var v_light_position = 18060;
    Var v_light_size = 50;
    Var v_math_min = 0.203173;
    Var v_sin = 5.100006;
    Var v_size = 3;
    Var v_size_100 = 300;
    Var v_size_2 = 9;
    Var v_speed = 1;
    Var v_tick = 88;
    Var v_timer = 30.396;
    Var v_up = 89700;

    Array l_accumulatedLight;
    Array l_accumulatedLight_blue;
    Array l_accumulatedLight_green;
    Array l_accumulatedLight_red;
    Array l_COLOR_SHIFT;
    Array l_GLASS_COLORS;
    Array l_imgData_blue;
    Array l_imgData_green;
    Array l_imgData_red;
    Array l_pixelMass;
    Array l_waveHeight;
    Array l_waveHeight_blue;
    Array l_waveHeight_green;
    Array l_waveHeight_red;
    Array l_waveVelocity;
    Array l_waveVelocity_blue;
    Array l_waveVelocity_green;
    Array l_waveVelocity_red;


    class Scripts {
    public:
        static void spriteProcedure_math_min(Arg arg_1, Arg arg_2) {
            if (arg_1 > arg_2) {
                v_math_min = arg_2;
            } else {
                v_math_min = arg_1;
		}
        }

        static void spriteProcedure_loop_unrolling_1() {
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
        }

        static void spriteProcedure_loop_unrolling_10() {
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
            spriteProcedure_loop_unrolling_1();
        }

        static void spriteProcedure_loop_unrolling_100() {
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
            spriteProcedure_loop_unrolling_10();
        }

        static void spriteProcedure_loop_unrolling_1000() {
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
            spriteProcedure_loop_unrolling_100();
        }

        static void spriteProcedure_loop_unrolling_10000() {
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
            spriteProcedure_loop_unrolling_1000();
        }

        static Coroutine spriteProcedure_calculate() {
            if (v_frame <= 300.0) {
                v_i = v_light_position;
                v_sin = degSin(fmod(v_frame * 45.83662439235437 - 90.0, 360.0)) * 12.0;

                repeat (v_light_size) {
                    v_i += 1.0;

                    l_waveHeight_red.set(v_i, v_sin);
                    l_waveHeight_green.set(v_i, v_sin);
                    l_waveHeight_blue.set(v_i, v_sin);

                    cs_pass;
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

                cs_pass;
            }

            v_i = 0;
            v_up = v_size_100 * -1.0;
            v_down = v_size_100;

            repeat (v_size_2) {
                spriteProcedure_loop_unrolling_10000();

                cs_pass;
            }

            v_frame += 1.0;

            co_return;
        }

        static Coroutine spriteProcedure_render() {
            BlockWindowUpdates blocker;
            
            Pen_safe(Pen::eraseAll());
            sprite.goXY(v_size_100 / 2.0 * -1.0, v_size_100 / 2.0);

            v_i = 0;

            repeat (v_size_100) {
                sprite.setY(v_size_100 / 2.0);
                sprite.penDown();

                repeat (v_size_100) {
                    v_i += 1.0;

                    spriteProcedure_math_min(l_accumulatedLight_red.get(v_i), 1.0);
                    v_colorValue = v_math_min * v_math_min * 255.0;

                    if (l_pixelMass.get(v_i) < 1.0) {
                        spriteProcedure_math_min(v_colorValue + l_GLASS_COLORS.get(1.0), 255.0);
                        v_colorValue = v_math_min;
                    }
                    l_imgData_red.set(v_i, v_colorValue);

                    spriteProcedure_math_min(l_accumulatedLight_green.get(v_i), 1.0);
                    v_colorValue = v_math_min * v_math_min * 255.0;

                    if (l_pixelMass.get(v_i) < 1.0) {
                        spriteProcedure_math_min(v_colorValue + l_GLASS_COLORS.get(2.0), 255.0);
                        v_colorValue = v_math_min;
                    }
                    l_imgData_green.set(v_i, v_colorValue);

                    spriteProcedure_math_min(l_accumulatedLight_blue.get(v_i), 1.0);
                    v_colorValue = v_math_min * v_math_min * 255.0;

                    if (l_pixelMass.get(v_i) < 1.0) {
                        spriteProcedure_math_min(v_colorValue + l_GLASS_COLORS.get(3.0), 255.0);
                        v_colorValue = v_math_min;
                    }
                    l_imgData_blue.set(v_i, v_colorValue);

                    sprite.penSetColor(round(l_imgData_red.get(v_i)) * 65536.0 + round(l_imgData_green.get(v_i)) * 256.0 + round(l_imgData_blue.get(v_i)));
                    sprite.changeY(-1);

                    cs_pass;
                }

                sprite.penUp();
                sprite.changeX(1);   

                cs_pass;
            }

            co_return;
        }

        static Coroutine spriteStartScript1(Context * ctx) {
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

                    cs_pass;
                }

                v_i += 1.0;

                cs_pass;
            }

            forever {
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_calculate();
                cs_wait spriteProcedure_render();
                v_tick += 1;

                cs_pass;
            }

            co_return;
        }

        static Coroutine spriteStartScript2(Context * ctx) {
            forever {
                wait_until(v_tick == 100);
                v_timer = csTime.timer();
                v_tick = 0;

                // don't have graphical output rn, so it's just like this
                wprintf(L"timer = %ls\n", v_timer.toString());

                cs_pass;
            }
            
            co_return;
        }
    };


    ScriptManager bindScripts({
        {ACTION_START, {
            {&sprite, {
                Scripts::spriteStartScript1,
                Scripts::spriteStartScript2,
            }},
        }}
    });
}

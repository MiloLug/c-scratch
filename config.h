#ifndef C_SCRATCH_CONFIG_H
#define C_SCRATCH_CONFIG_H

/**** COMMON ****/
// Enables some useful outputs such as "sprite X is initialized"
#define DEBUG
// Enables "turbo" mode. No FPS limits will be applied
#define ENABLE_TURBO
#define NON_TURBO_CALCULATION_FPS 60

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 360

/**** ARRAYS ****/
// In C we have to allocate space before inserting the items
// So this is by what multiply an array's size each time it reaches its bounds
#define ARRAY_AHEAD_ALLOCATION_MULTIPLIER 1.5
#define ARRAY_INITIAL_SIZE 4

/**** MATH ****/
// Enables fast trig functions. The only disadvantage is the accuracy
// in some functions, but it's enough for almost any calculations
#define MATH_USE_FAST_TRIGONOMETRY

#endif

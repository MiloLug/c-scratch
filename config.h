#ifndef C_SCRATCH_CONFIG_H
#define C_SCRATCH_CONFIG_H

/**** COMMON ****/
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Enables some useful outputs such as "sprite X is initialized"
#define DEBUG
// Enables "turbo" mode. No FPS limits will be applied
#define ENABLE_TURBO
#define NON_TURBO_CALCULATION_FPS 60
/*
* It determines, for how many cycles to lock some resources.
* For example, when we use Pen in non-turbo mode, it says 'im working with the canvas',
*    but it takes too much time to say this 1000'000+ times.
* So in the turbo mode, main function will say 'im working with the canvas',
*    then run 10000 operations and say 'the canvas is ready to be shown'.
*/
#define TURBO_LOCK_WINDOW_CYCLES (WINDOW_WIDTH * WINDOW_HEIGHT + 1)

/*
* Enables a mode in which some threads-syncronization mechanisms can be disabled.
* It TREMENDOUSLY speeds up such tools as pen,
* but in *some* cases the app may crash (but it's highly unlikely as tests show).
*/
#define ENABLE_UNSAFE_NO_LOCKS

// How many of the fraction part to save in some "number to string" operations
#define NUM_TO_STRING_FRACTION_DIGITS 20

/**** ARRAYS ****/
/*
* In C we have to allocate some space before inserting the items
* So this is by what number to multiply an array's size each time it reaches its bounds
*/
#define ARRAY_AHEAD_ALLOCATION_MULTIPLIER 1.5
#define ARRAY_INITIAL_SIZE 4

/**** MATH ****/
/*
* Enables fast math functions. The only disadvantage is the accuracy
* in some functions, but it's enough for almost any calculations
*/
#define USE_FAST_MATH

#endif

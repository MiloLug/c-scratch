#ifndef CSCRATCH_CONFIG_H
#define CSCRATCH_CONFIG_H

/**** COMMON ****/
#define APP_WINDOW_NAME "Light simulation project"

#define WINDOW_WIDTH 480.0f
#define WINDOW_HEIGHT 360.0f

// Enables "turbo" mode. No FPS limits will be applied
#define ENABLE_TURBO
#define NON_TURBO_CALCULATION_FPS 60
/*
* It determines, for how many cycles to lock some resources.
* For example, when we use Pen in non-turbo mode, it says 'im working with the canvas',
*   but it takes too much time to say this 1000'000+ times.
* So in the turbo mode, main function will say 'im working with the canvas',
*   then run 10000 operations and say 'the canvas is ready to be shown'.
*/
#define TURBO_LOCK_WINDOW_CYCLES (10000)

/*
* Enables a mode in which some threads-syncronization mechanisms can be disabled.
* It TREMENDOUSLY speeds up such tools as pen,
*   but in *some* cases the app may crash (but it's highly unlikely as tests show).
*/
#define ENABLE_UNSAFE_NO_LOCKS

/*
* Disables limits on the sprites coordinates
* Consequences: overflows (and undefined behaviour of some calculation) in case you set them to 
*   numbers higher then 4294967084
*/
#define NO_COORDINATES_LIMITATION


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
*   in some functions, but it's enough for almost any calculations
*/
#define ENABLE_FAST_MATH

/*
* Make the Value (storage type for strings an number) to use float instead of double.
* But be aware the doubles can store much larger numbers, not losing
*   speed that much. Its about a second on hundreds of millions of updates.
*
* Also the original scratch is written in JS, where numbers are doubles by default, so it can be
*   incompatible with some projects in this case
*/
#define USE_VALUE_FLOAT

/**** DEBUGGING ****/
// Enables some useful outputs such as "sprite X is initialized"
#define DEBUG

// Enables semi-transparent boxes for the Pen::stamp
// #define PEN_STAMP_DEBUG

#endif

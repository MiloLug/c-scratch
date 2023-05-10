#ifndef CSCRATCH_PEN_SAFE_H
#define CSCRATCH_PEN_SAFE_H

#include "config.h"


#if !defined ENABLE_TURBO && !defined ENABLE_UNSAFE_NO_LOCKS
    /*
    * Blocks the canvas for rendering while executing the code.
    *
    * After execution, sets `hasChanges` variable to tell the renderer to update the image.
    * Returns 1
    */
    #define Pen_safe(code)                                                                         \
        Pen::pixels.take();                                                                        \
        code;                                                                                      \
        Pen::hasChanges = true;                                                                    \
        Pen::pixels.release()
#else
    /*
    * Should block the canvas for rendering while executing the code
    * BUT you have `ENABLE_TURBO` or `ENABLE_UNSAFE_NO_LOCKS`, so it doesn't
    *
    * After execution, sets `hasChanges` variable to tell the renderer to update the image.
    * Returns 1
    */
    #define Pen_safe(code)                                                                         \
        code;                                                                                      \
        Pen::hasChanges = true
#endif


#endif

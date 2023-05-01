#ifndef CSCRATCH_CONTROL_FLOW_H
#define CSCRATCH_CONTROL_FLOW_H

#include "coroutines.h"


#define waitUntil(expr) while(!(expr)) co_yield NULL
#define repeatUntil(expr) while(!(expr))

#define repeat(expr) for(double __i = 0, __limit = round(expr); __i < __limit; __i++)

#define forever while(1)

#define stopAll() shouldRun = false; co_yield NULL
#define stopThisScript() co_return

//TODO: implement!
#define stopThisSprite() ({1;})


static inline bool __coroNext(const Coroutine &subCoro) {
    if (!subCoro.done()) {
        subCoro.resume();
        return !subCoro.done();
    }
    return false;
}
static inline bool __coroNext(void *) { return true; }

#define cs_wait(coroExpr)             \
    do {                           \
        auto __tmp = (coroExpr);   \
        if (__coroNext(__tmp))     \
            co_yield __tmp;        \
    } while(0)

#define cs_yield co_yield NULL


#endif

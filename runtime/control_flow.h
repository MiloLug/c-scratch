#ifndef CSCRATCH_CONTROL_FLOW_H
#define CSCRATCH_CONTROL_FLOW_H

#include "coroutines.h"
#include "time.h"

#define repeatUntil(expr) while (!(expr))
#define waitUntil(expr) repeatUntil(expr) co_yield NULL

#define waitFor(seconds)                                                                           \
    for (double end = programTime + (double)(seconds); programTime < end;) co_yield NULL

#define repeat(expr) for (double __i = 0, __limit = round(expr); __i < __limit; __i++)

#define forever while (1)

#define stopAll()                                                                                  \
    ScriptManager::shouldRun = false;                                                              \
    co_return
#define stopThisScript() co_return


#define cs_wait(coroExpr)                                                                          \
    do {                                                                                           \
        auto __tmp = (coroExpr);                                                                   \
        if constexpr (std::is_same_v<decltype(__tmp), Coroutine>) {                                \
            __tmp.resume();                                                                        \
            if (!__tmp.done()) co_yield __tmp;                                                     \
        } else {                                                                                   \
            co_yield __tmp;                                                                        \
        }                                                                                          \
    } while (0)

#define cs_yield co_yield NULL


#endif

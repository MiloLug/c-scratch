#ifndef CSCRATCH_CONTROL_FLOW_H
#define CSCRATCH_CONTROL_FLOW_H

#include "coroutines.h"
#include "time.h"

#define repeatUntil(expr) while (!(expr))
#define waitUntil(expr) repeatUntil(expr) co_yield NULL

#define waitFor(seconds)                                                                           \
    for (const int64_t end = programTime + time_sToNS(seconds); programTime < end;) co_yield NULL

#define repeat(expr) for (double __i = 0, __limit = round(expr); __i < __limit; __i++)

#define forever while (1)

#define stopAll()                                                                                  \
    ScriptManager::shouldRun = false;                                                              \
    co_return
#define stopThisScript() co_return


force_inline__ const Coroutine & __coroPreWait(const Coroutine & coro, bool & done) {
    coro.resume();
    done = coro.done();
    return coro;
}

template<typename T>
force_inline__ T & __coroPreWait(T & value, bool & done) {
    done = false;
    return value;
}


#define cs_wait co_yield

#define cs_yield co_yield

#define cs_pass co_yield Coroutine::NOTHING


#endif

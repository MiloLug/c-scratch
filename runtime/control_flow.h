#ifndef CSCRATCH_CONTROL_FLOW_H
#define CSCRATCH_CONTROL_FLOW_H

#include "coroutines.h"
#include "time.h"

#define repeatUntil(expr) while (!(expr))
#define waitUntil(expr) repeatUntil(expr) co_yield Coroutine::NOTHING

#define waitFor(seconds)                                                                           \
    for (const int64_t end = programTime + time_sToNS(seconds); programTime < end;)                \
    co_yield Coroutine::NOTHING

#define repeatFor(seconds)                                                                         \
    for (const int64_t end = programTime + time_sToNS(seconds); programTime < end;)                \
    co_yield Coroutine::NOTHING

#define repeat(expr) for (double __i = 0, __limit = round(expr); __i < __limit; __i++)

#define forever while (1)


#define cs_stop_all                                                                                \
    ScriptManager::shouldRun = false;                                                              \
    co_return

#define cs_stop co_return

#define cs_wait co_yield

#define cs_yield co_yield

#define cs_pass co_yield Coroutine::NOTHING


#endif

#ifndef CSCRATCH_CONTROL_FLOW_H
#define CSCRATCH_CONTROL_FLOW_H

#include "coroutines.h"
#include "script/should_run.h"
#include "time.h"

#define repeat_until(expr) while (!(expr))
#define wait_until(expr) repeat_until(expr) co_yield Coroutine::NOTHING

#define wait_for(seconds)                                                                          \
    for (const int64_t end = programTime + time_sToNS(seconds); programTime < end;)                \
    co_yield Coroutine::NOTHING

#define repeat_for(seconds)                                                                        \
    for (const int64_t end = programTime + time_sToNS(seconds); programTime < end;)                \
    co_yield Coroutine::NOTHING

#define repeat(expr) for (double __i = 0, __limit = round(expr); __i < __limit; __i++)

#define forever while (1)


#define cs_stop_all                                                                                \
    ScriptsShouldRun = false;                                                                      \
    co_return

#define cs_stop co_return

#define cs_wait co_yield

#define cs_yield co_yield

#define cs_pass co_yield Coroutine::NOTHING


#endif

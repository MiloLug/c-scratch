#ifndef CSCRATCH_CONTROL_FLOW_H
#define CSCRATCH_CONTROL_FLOW_H

#include "coroutines.h"
#include "script/should_run.h"
#include "time.h"

#define cs_repeat_until(expr) while (!(expr))
#define cs_wait_until(expr) cs_repeat_until(expr) co_yield Coroutine::NOTHING

#define cs_wait_for(seconds)                                                                       \
    for (const int64_t end = csTime.programTime + Time::sToNS(seconds); csTime.programTime < end;) \
    co_yield Coroutine::NOTHING

#define cs_repeat_for(seconds)                                                                     \
    for (const int64_t end = csTime.programTime + Time::sToNS(seconds); csTime.programTime < end;) \
    co_yield Coroutine::NOTHING

#define cs_repeat(expr) for (double __i = 0, __limit = round(expr); __i < __limit; __i++)

#define cs_forever while (1)


#define cs_stop_all                                                                                \
    ScriptsShouldRun = false;                                                                      \
    co_return

#define cs_stop co_return

#define cs_wait co_yield

#define cs_yield co_yield

#define cs_pass co_yield Coroutine::NOTHING

template<class Fn>
class cs_defer {
protected:
    Fn deferred;

public:
    cs_defer(Fn && f): deferred{f} {}

    ~cs_defer() { deferred(); }
};

#endif

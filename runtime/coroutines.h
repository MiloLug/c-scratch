#ifndef COROUTINES_H
#define COROUTINES_H

#include "config.h"
#include "script/should_run.h"
#include "utils.h"

#if __has_include(<coroutine>)
    #include <coroutine>
#else
    #include <experimental/coroutine>
namespace std {
    using std::experimental::coroutine_handle;
    using std::experimental::suspend_always;
    using std::experimental::suspend_never;
}
#endif

using std::coroutine_handle;

struct CoroPromise;


struct Coroutine: coroutine_handle<CoroPromise> {
    struct DoesSuspend {
        bool suspend;
        constexpr void await_suspend(coroutine_handle<>) const noexcept {}
        constexpr void await_resume() const noexcept {}
        constexpr bool await_ready() const noexcept { return !suspend; }
    };

    struct Nothing {};
    static constexpr const Nothing * NOTHING = nullptr;

    using promise_type = ::CoroPromise;

    ~Coroutine() { destroy(); }

    void resume() const;
};


struct CoroPromise {
    const Coroutine * subCoro = nullptr;

    Coroutine get_return_object() { return {Coroutine::from_promise(*this)}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}

    std::suspend_always yield_value(const Coroutine::Nothing *) noexcept { return {}; }

    Coroutine::DoesSuspend yield_value(Coroutine && _subCoro) noexcept {
        // This allows to not exit to the main loop when the sub coroutine finishes in one go
        // and thus "interruption time" will be smaller
        if (_subCoro.done()) {
            // but even then, the coroutine shouldn't run when ScriptsShouldRun = false
            return {!ScriptsShouldRun};
        }
        subCoro = &_subCoro;
        return {true};
    }
};


inline void Coroutine::resume() const {
    auto & subCoro = promise().subCoro;
    if (subCoro && !subCoro->done()) {
        subCoro->resume();
        return;
    }

    subCoro = nullptr;
    coroutine_handle<CoroPromise>::resume();
}


struct Context {};


#endif

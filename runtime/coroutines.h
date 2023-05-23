#ifndef COROUTINES_H
#define COROUTINES_H

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

struct BasePromise;


struct Coroutine: coroutine_handle<BasePromise> {
    struct DoesSuspend: public std::suspend_always {
        bool suspend;

        constexpr DoesSuspend(bool _suspend = true): suspend{_suspend}, std::suspend_always{} {}

        constexpr bool await_ready() const noexcept { return !suspend; }
    };

    struct Nothing {};
    static constexpr const Nothing * NOTHING = nullptr;

    using promise_type = ::BasePromise;

    ~Coroutine() { destroy(); }

    void resume() const;
};


struct BasePromise {
    const Coroutine * subCoro = nullptr;

    Coroutine get_return_object() { return {Coroutine::from_promise(*this)}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
    std::suspend_always yield_value(const Coroutine::Nothing *) { return {}; }
    Coroutine::DoesSuspend yield_value(Coroutine && _subCoro) {
        if (_subCoro.done()) {
            return {false};
        }
        subCoro = &_subCoro;
        return {true};
    }
};


inline void Coroutine::resume() const {
    auto & subCoro = promise().subCoro;
    if (subCoro && !subCoro->done()) {
        subCoro->resume();
    } else {
        subCoro = nullptr;
        coroutine_handle<BasePromise>::resume();
    }
}


struct Context {};


#endif

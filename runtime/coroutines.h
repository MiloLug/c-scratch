#ifndef COROUTINES_H
#define COROUTINES_H

#if __has_include(<coroutine>)
    #include <coroutine>
#else
    #include <experimental/coroutine>
    namespace std {
        using std::experimental::suspend_always;
        using std::experimental::suspend_never;
        using std::experimental::noop_coroutine;
        using std::experimental::coroutine_handle;
    }
#endif

struct BasePromise;


struct Coroutine : std::coroutine_handle<BasePromise>
{
    using promise_type = ::BasePromise;

    ~Coroutine() {
        destroy();
    }

    void resume() const;
};


struct BasePromise
{
public:
    const Coroutine * subCoro = NULL;

    Coroutine get_return_object() { return {Coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
    std::suspend_always yield_value(void *) { return {}; }
    std::suspend_always yield_value(const Coroutine &_subCoro) { 
        subCoro = &_subCoro;
        return {};
    }
};


inline void Coroutine::resume() const {
    auto &subCoro = promise().subCoro;
    if (subCoro && !subCoro->done()) {
        subCoro->resume();
    } else {
        subCoro = NULL;
        std::coroutine_handle<BasePromise>::resume();
    }
}


struct Context {};


#endif

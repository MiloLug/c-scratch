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

struct VoidCoroutine : std::coroutine_handle<BasePromise>
{
    using promise_type = ::BasePromise;

    ~VoidCoroutine() {
        destroy();
    }
};

struct BasePromise
{
    VoidCoroutine get_return_object() { return {VoidCoroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
    std::suspend_always yield_value(void *) { return {}; }
};


#endif

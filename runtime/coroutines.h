#ifndef COROUTINES_H
#define COROUTINES_H

#if __has_include(<coroutine>) && !defined(COMPILER_MSVC)
    #include <coroutine>
#else
    #include <experimental/coroutine>
namespace std {
    using std::experimental::coroutine_handle;
    using std::experimental::suspend_always;
    using std::experimental::suspend_never;
}
#endif

#if defined(COMPILER_MSVC)
template <typename _PromiseT>
struct coroutine_handle : std::coroutine_handle<> { // general form
    using std::coroutine_handle<>::coroutine_handle;

    __declspec(deprecated("with pointer parameter is deprecated. Use coroutine_handle::from_promise(T&) "
        "instead")) static coroutine_handle from_promise(_PromiseT* _Prom) noexcept {
        return from_promise(*_Prom);
    }

    static coroutine_handle from_promise(_PromiseT& _Prom) noexcept {
        auto _FramePtr = reinterpret_cast<char*>(_STD addressof(_Prom)) + _ALIGNED_SIZE;
        coroutine_handle<_PromiseT> _Result;
        _Result._Ptr = reinterpret_cast<_Resumable_frame_prefix*>(_FramePtr);
        return _Result;
    }

    static coroutine_handle from_address(void* _Addr) noexcept {
        coroutine_handle _Result;
        _Result._Ptr = static_cast<_Resumable_frame_prefix*>(_Addr);
        return _Result;
    }

    coroutine_handle& operator=(nullptr_t) noexcept {
        _Ptr = nullptr;
        return *this;
    }

    static const size_t _ALIGN_REQ = sizeof(void*) * 2;

    static const size_t _ALIGNED_SIZE;

    _PromiseT& promise() const noexcept {
        return *const_cast<_PromiseT*>(
            reinterpret_cast<const _PromiseT*>(reinterpret_cast<const char*>(_Ptr) - _ALIGNED_SIZE));
    }
};
#else
using std::coroutine_handle;
#endif


struct BasePromise;


struct Coroutine: coroutine_handle<BasePromise> {
    struct DoesSuspend: public std::suspend_always {
        bool suspend;

        DoesSuspend(bool _suspend = true): suspend{_suspend}, std::suspend_always{} {}

        constexpr bool await_ready() const noexcept { return !suspend; }
    };

    struct Nothing {};
    static constexpr const Nothing * NOTHING = nullptr;

    using promise_type = ::BasePromise;

    ~Coroutine() { destroy(); }

    void resume() const;
};

struct BasePromise {
public:
    const Coroutine * subCoro = nullptr;

    Coroutine get_return_object() { return {Coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
    std::suspend_always yield_value(const Coroutine::Nothing *) { return {}; }
    Coroutine::DoesSuspend yield_value(Coroutine && _subCoro) {
        _subCoro.resume();
        if (_subCoro.done()) {
            return {false};
        }
        subCoro = &_subCoro;
        return {true};
    }
};

#if defined(COMPILER_MSVC)
const size_t Coroutine::_ALIGNED_SIZE = ((sizeof(BasePromise) + Coroutine::_ALIGN_REQ - 1) & ~(Coroutine::_ALIGN_REQ - 1));
#endif

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

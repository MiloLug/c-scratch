#ifndef CSCRATCH_THREAD_H
#define CSCRATCH_THREAD_H

#include <functional>
#if __has_include(<pthread.h>)
    #include <pthread.h>
#else
    #include <thread>
    #define USE_STD_THREAD
#endif


#ifdef USE_STD_THREAD
using Thread = std::thread;
#else
class Thread {
protected:
    struct State {
        std::function<void()> threadFn;
    };

    pthread_t thr;

    static void * runInThreadHelper(void * ctx) {
        ((State *)ctx)->threadFn();
        delete (State *)ctx;
        return nullptr;
    }

public:

    template<typename... Args>
    Thread(std::invocable<Args...> auto &&fn, Args &&... args) {
        auto thrFn = std::bind(fn, args...);
        pthread_create(&thr, NULL, &Thread::runInThreadHelper, new State{[thrFn] {
            thrFn();
        }});
    }

    ~Thread() {
        if (joinable()) join();
    }

    Thread(const Thread &) = delete;

    Thread(Thread && __t) noexcept { swap(__t); }

    Thread & operator=(const Thread &) = delete;

    Thread & operator=(Thread && __t) noexcept {
        if (joinable()) join();
        swap(__t);
        return *this;
    }

    void swap(Thread & _t) noexcept { std::swap(thr, _t.thr); }

    bool joinable() const noexcept { return thr != pthread_self(); }

    void join() { pthread_join(thr, nullptr); }
    void detach() { pthread_detach(thr); }
    pthread_t get_id() {
        return thr;
    }
};
#endif

#endif

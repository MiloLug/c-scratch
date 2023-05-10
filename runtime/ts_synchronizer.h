#ifndef THREAD_SAFE_SYNCHRONIZER_H
#define THREAD_SAFE_SYNCHRONIZER_H

#include <atomic>
#include <cstdint>
#include <queue>


class ThreadSafeSynchronizer {
private:
    std::atomic<uint32_t> counter = 0;

public:
    void take() { counter++; }
    void release() { counter--; }
    void wait() {
        uint32_t oldCounter = 0;
        do {
            oldCounter = 0;
        } while (!counter.compare_exchange_weak(oldCounter, oldCounter));
    }
    bool is_blocked() { return counter.load(); }
};


#endif

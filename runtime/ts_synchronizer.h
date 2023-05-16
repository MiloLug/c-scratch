#ifndef THREAD_SAFE_SYNCHRONIZER_H
#define THREAD_SAFE_SYNCHRONIZER_H

#include <atomic>
#include <cstdint>
#include <queue>
#include "mutex.h"


class ThreadSafeSynchronizer {
private:
    std::atomic<uint32_t> counter = 0;
    Mutex inProcessing;
    
public:
    // Call take and release on a thread that makes some changes
    void take() {
        inProcessing.take();
        counter++;
        inProcessing.release();
    }
    void release() { counter--; }

    // Call start and stop processing on the changes processing thread
    void startProcessing() { inProcessing.take(); }
    void stopProcessing() { inProcessing.release(); }

    void wait() {
        uint32_t oldCounter = 0;
        do {
            oldCounter = 0;
        } while (!counter.compare_exchange_weak(oldCounter, oldCounter));
    }

    bool is_blocked() { return counter.load(); }
};


#endif

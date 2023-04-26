#ifndef THREAD_SAFE_CONTAINER_H
#define THREAD_SAFE_CONTAINER_H

#include <atomic>
#include <cstdint>


template<typename T>
class ThreadSafeContainer
{
private:
    std::atomic<bool> taken;
public:
    volatile T value;
    
    ThreadSafeContainer(T &&_value): value(_value) {}

    void take() {
        bool isTaken;

        do {
            isTaken = false;
        } while (!taken.compare_exchange_weak(isTaken, true));
    }

    void release() {
        bool isTaken = true;
        while (!taken.compare_exchange_weak(isTaken, false));
    }
};


#endif

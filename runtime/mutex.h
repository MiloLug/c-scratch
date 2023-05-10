#ifndef CSCRATCH_MUTEX_H
#define CSCRATCH_MUTEX_H

#include <atomic>


class Mutex {
private:
    std::atomic<bool> taken;

public:
    void take() {
        bool isTaken;

        do {
            isTaken = false;
        } while (!taken.compare_exchange_weak(isTaken, true));
    }

    void release() {
        bool isTaken = true;
        while (!taken.compare_exchange_weak(isTaken, false))
            ;
    }
};

#endif

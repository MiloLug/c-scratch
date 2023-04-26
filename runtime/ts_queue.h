#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <atomic>
#include <cstdint>


template<typename T>
class ThreadSafeQueue
{
private:
    typedef std::queue<T> qtype;

    std::atomic<uint32_t> size = 0;
    qtype _queue;
    std::atomic<bool> taken;
public:

    void takeQueue() {
        bool isTaken;

        do {
            isTaken = false;
        } while (!taken.compare_exchange_weak(isTaken, true));
    }

    void releaseQueue() {
        bool isTaken = true;
        while (!taken.compare_exchange_weak(isTaken, false));
    }

    void push(T const& data) {
        takeQueue();

        _queue.push(data);
        size++;

        releaseQueue();
    }

    T pop(T &out) {
        uint32_t oldSize = size.load();
        do {
            if (oldSize == 0) return false;
        } while (!size.compare_exchange_weak(oldSize, oldSize - 1));
        
        takeQueue();
        
        out = _queue.front();
        _queue.pop();
        
        releaseQueue();
        return true;
    }

    T pop() {
        uint32_t oldSize = size.load();
        do {
            if (oldSize == 0) return T{0};
        } while (!size.compare_exchange_weak(oldSize, oldSize - 1));

        takeQueue();
        
        T tmp = _queue.front();
        _queue.pop();
        
        releaseQueue();
        return tmp;
    }
};


#endif

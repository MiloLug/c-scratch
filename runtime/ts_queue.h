#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <atomic>
#include <queue>
#include <mutex>
#include <atomic>
#include <cstdint>


template<typename T>
class ThreadSafeQueue
{
private:
    std::atomic<uint32_t> size = 0;
    std::queue<T> _queue;
    std::mutex mut;
public:
    void push(T const& data) {
        mut.lock();
        _queue.push(data);
        size++;
        mut.unlock();
    }
    T pop() {
        uint32_t oldSize = size.load();
        do {
            if (oldSize == 0) return NULL;
        } while (!size.compare_exchange_weak(oldSize, oldSize - 1));

        mut.lock();
        T tmp = _queue.front();
        _queue.pop();
        mut.unlock();
        return tmp;
    }
};


#endif

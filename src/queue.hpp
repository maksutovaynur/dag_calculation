//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_CQUEUE_H
#define DAG_CALCULATION_CQUEUE_H


#include "queue"
#include "mutex"
#include "optional"


template <typename T>
class ConcurrentQueue {
public:
    void push(T element) {
        mutex.lock();
        queue.push(element);
        mutex.unlock();
    }
    std::optional<T> pop() {
        mutex.lock();
        T v;
        bool hasValue = false;
        if (! queue.empty()) {
            v = queue.front();
            queue.pop();
            hasValue = true;
        };
        mutex.unlock();
        if (hasValue) return std::optional<T>(v);
        else return std::nullopt;
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
};


#endif //DAG_CALCULATION_CQUEUE_H

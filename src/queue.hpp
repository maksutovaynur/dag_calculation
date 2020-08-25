//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_CQUEUE_H
#define DAG_CALCULATION_CQUEUE_H


#include "queue"
#include "mutex"


template <typename T>
class ConcurrentQueue {
public:
    void push(T element) {
        mutex.lock();
        queue.push(element);
        mutex.unlock();
    }
    T pop() {
        mutex.lock();
        T v;
        try {
            v = queue.back();
            queue.pop();
        } catch ( std::exception ) {
            mutex.unlock();
            throw "Queue empty";
        }
        mutex.unlock();
        return v;
    }

private:
    std::queue<T> queue;
    std::mutex mutex;
};


#endif //DAG_CALCULATION_CQUEUE_H

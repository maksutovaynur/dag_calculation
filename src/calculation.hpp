//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_THREADPOOL_H
#define DAG_CALCULATION_THREADPOOL_H

#include "cell.hpp"
#include "thread"
#include "vector"


void calculationCycle(CellQueue *queue) {
    QueueTask task;
    while (true) {
        try {
            task = queue->pop();
        } catch ( std::exception ) {
            break;
        }
        task.cell->writeLock();
        task.cell->calculate(task.version);
        task.cell->writeUnlock();
    }

}


class CellThreadPool {
public:
    CellThreadPool(CellQueue *queue, int threadCount) {
        for (int i = 0; i < threadCount; i ++) {
            threads.push_back(move(std::thread(calculationCycle, std::ref(queue))));
        }
    }
    void join() {
        for (auto &t : threads) {
            t.join();
        }
    }
private:
    std::vector<std::thread> threads;
};


#endif //DAG_CALCULATION_THREADPOOL_H

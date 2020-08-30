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
        auto taskOpt = queue->pop();
        if (!taskOpt.has_value()) break;
        task = taskOpt.value();
        task.cell->writeLock();
        task.cell->calculate(task.version);
        for (auto &it : *task.cell) {
            queue->push(QueueTask{ it, task.version });
        }
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
    int getThreadCount() {
        return threads.size();
    }
private:
    std::vector<std::thread> threads;
};


#endif //DAG_CALCULATION_THREADPOOL_H

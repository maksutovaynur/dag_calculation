//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_THREADPOOL_H
#define DAG_CALCULATION_THREADPOOL_H

#include "cell.hpp"
#include "thread"
#include "vector"


void calculationCycle(CellQueue *queue) {
    while (true) {
        auto taskOpt = queue->pop();
        if (!taskOpt.has_value()) break;
        QueueTask & task = taskOpt.value();
        auto & cell = task.cell;
        CalcVersion version = task.version;
        if (cell->getVersion() >= version) continue;
        cell->writeLock();
        cell->calculate(version);
        for (auto &it : *cell) queue->push(QueueTask{ it, version });
        cell->writeUnlock();
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

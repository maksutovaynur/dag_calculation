#ifndef DAG_CALCULATION_CNODE_H
#define DAG_CALCULATION_CNODE_H


#include "unordered_set"
#include "vector"
#include "shared_mutex"
#include "types.hpp"
#include "queue.hpp"
#include "unistd.h"


class Cell {
public:
    CalcVersion getVersion() {
        this->versionMutex.lock_shared();
        auto v = version;
        this->versionMutex.unlock_shared();
        return v;
    }
    CellType getType() {
        return type;
    }
    CellState getState() {
        return state;
    }
    void setArgument(ArgType argument) {
        arg = argument;
    }
    ResType getResult() {
        return result;
    }
    void setRefs(std::vector<Cell*> &vector){
        rmRefs();
        for (Cell *node : vector) {
            node->addOuterRef(this);
            refs.push_back(node);
        }
//        std::cout << "Refs count: " << refs.size() << std::endl;
        if (!refs.empty())
            type = FORMULA;
        else
            type = VALUE;
    }
    void rmRefs(){
        for (auto & r : refs) {
            r->rmOuterRef(this);
        }
        refs.clear();
        type = VALUE;
    }
    void addOuterRef(Cell *node) {
        if (node == this) throw "CyclicRef: can not add Ref to self";
        outerRefs.insert(node);
    }
    void rmOuterRef(Cell *node) {
        outerRefs.erase(node);
    }
    void setState(CellState newState) {
        state = newState;
    }
    bool calculate(CalcVersion cVersion) {
        if (version >= cVersion) return false;
//        std::cout << "Cell type = " << type << std::endl;
        if (type == FORMULA) {
            ResType tmp = 0;
            for (Cell *node: refs) {
                switch (node->getState()) {
                    case DONE:
                        tmp += node->result;
                        break;
                    case NO_VALUE:
                        state = NO_VALUE;
                        return true;
                }
            }
            result = tmp;
            usleep(1000);
        } else result = arg;
        version = cVersion;
        state = DONE;
        return true;
    }

    std::unordered_set<Cell*>::iterator begin() noexcept {
        return outerRefs.begin();
    }
    std::unordered_set<Cell*>::iterator end() noexcept {
        return outerRefs.end();
    }
    void readLock() {
        this->mutex.lock_shared();
    }
    void readUnlock() {
        this->mutex.unlock_shared();
    }
    void writeLock() {
        this->mutex.lock();
    }
    void writeUnlock() {
        this->mutex.unlock();
    }
    Cell(){
        state = NO_VALUE;
        version = 0;
        arg = 0;
        result = 0;
        type = VALUE;
    }
    void clean() {
        arg = 0;
        version = 0;
        result = 0;
        type = VALUE;
        rmRefs();
    }
private:
    CellState state;
    CellType type;
    CalcVersion version;
    std::shared_mutex versionMutex;
    std::shared_mutex mutex;
    ArgType arg;
    ResType result;
    std::vector<Cell*> refs;
    std::unordered_set<Cell*> outerRefs;
};


struct QueueTask {
    Cell *cell;
    CalcVersion version;
};

#define CellQueue ConcurrentQueue<QueueTask>
#endif
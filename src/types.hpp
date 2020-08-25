//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_ARGS_H
#define DAG_CALCULATION_ARGS_H

#include "shared_mutex"

#define CalcVersion unsigned int
#define ArgType long long int
#define ResType long long int


enum CellType {
    VALUE,
    FORMULA
};


enum CellState {
    NO_VALUE,
    DONE
};


class CalcVersionHolder {
public:
    static CalcVersion getVersion(){
        mutex.lock_shared();
        CalcVersion v = version;
        mutex.unlock_shared();
        return v;
    }
    static CalcVersion incVersion(){
        mutex.lock();
        CalcVersion v = ++version;
        mutex.unlock();
        return v;
    }

//private:
    static CalcVersion version;
    static std::shared_mutex mutex;
};


#endif //DAG_CALCULATION_ARGS_H

//
// Created by aynur on 01.09.2020.
//

#ifndef DAG_CALCULATION_MTIME_H
#define DAG_CALCULATION_MTIME_H

#include <chrono>

#define ptime std::chrono::steady_clock::time_point
#define now() std::chrono::steady_clock::now()
#define durationCast(x) std::chrono::duration_cast<std::chrono::microseconds>(x).count()

#endif //DAG_CALCULATION_MTIME_H

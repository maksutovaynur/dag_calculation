cmake_minimum_required(VERSION 3.17)
project(dag_calculation CXX)

set(CMAKE_CXX_STANDARD 17)
#find_library(pthread REQUIRED)


add_executable(dag_calculation
        src/main.cpp
        src/types.hpp/
        src/calculation.hpp
        src/graph.hpp
        src/cell.hpp
        src/parser.hpp
        src/queue.hpp
        )
#target_link_libraries(dag_calculation cnode)
#target_link_libraries(dag_calculation cgraph)
target_link_libraries(dag_calculation pthread)

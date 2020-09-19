cmake_minimum_required(VERSION 3.17)
project(dag_calculation CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(dag_calculation
        src/main.cpp
        src/types.hpp
        src/mtime.h
        src/parser.hpp
        src/queue.hpp
        src/graph.hpp)

target_link_libraries(dag_calculation pthread)

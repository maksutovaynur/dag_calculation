#include "fstream"
#include "iostream"
#include "string"
#include "graph.hpp"
#include "calculation.hpp"
#include <ctime>



void test1(std::string & filename) {
    auto graph = CellGraph();
    std::string input;
    std::ifstream file(filename);
    while (std::getline(file, input)) {
        graph.addCell((char*)input.c_str());
    }
    graph.stopInitialBuilding();
    auto start = std::time(nullptr);
    auto pool = CellThreadPool(graph.getQueue(), 1);
    pool.join();
    auto end = std::time(nullptr);
    std::cout << pool.getThreadCount() << " threads: " << end - start << " s" << std::endl;
    for (auto &it : graph) {
        if (it.second.getState() == DONE) {
            std::cout << it.first << " = " << it.second.getResult() << std::endl;
        }
        else {
            std::cout << it.first << " = UNDEFINED" << std::endl;
        }
    }
}


int main(int argc, char* argv[]) {
    std::string name;
    int threadCount;
    if (argc <= 1) {
        std::cin >> name;
    } else {
        name.append(argv[1]);
    }
    if (argc <= 2) {
        std::cin >> threadCount;
    } else {
        threadCount = std::stoi(argv[2]);
    }
    test1(name);
    return 0;
}

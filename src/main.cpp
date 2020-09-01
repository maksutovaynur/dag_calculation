#include "fstream"
#include "iostream"
#include "string"
#include "graph.hpp"
#include "calculation.hpp"
#include "mtime.h"



void test1(std::string & inFileName, std::string & outFileName, int threadCount) {
    auto graph = CellGraph();
    std::string input;
    std::ifstream inFile(inFileName);
    while (std::getline(inFile, input)) {
        graph.addCell((char*)input.c_str());
    }
    graph.stopInitialBuilding();
    std::cout << "Graph size: " << graph.size() << std::endl;
    ptime pstart = now();
    auto pool = CellThreadPool(graph.getQueue(), threadCount);
    pool.join();
    ptime pend = now();
    std::cout << pool.getThreadCount() << " threads: " << ((float)durationCast(pend - pstart))/1000000 << " s" << std::endl;
    std::ofstream outFile(outFileName);
    for (auto &it : graph) {
        if (it.second.getState() == DONE) {
            outFile << it.first << " = " << it.second.getResult() << std::endl;
        }
        else {
            outFile << it.first << " = UNDEFINED" << std::endl;
        }
    }
}


int main(int argc, char* argv[]) {
    std::string inFileName, outFileName;
    int threadCount;
    if (argc <= 1) {
        std::cin >> inFileName;
    } else {
        inFileName.append(argv[1]);
    }
    if (argc <= 2) {
        std::cin >> outFileName;
    } else {
        outFileName.append(argv[2]);
    }
    if (argc <= 3) {
        std::cin >> threadCount;
    } else {
        threadCount = std::stoi(argv[3]);
    }
    test1(inFileName, outFileName, threadCount);
    return 0;
}

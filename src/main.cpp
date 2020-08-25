#include "fstream"
#include "iostream"
#include "string"
#include "graph.hpp"
#include "calculation.hpp"



void test1(std::string & filename) {
    auto graph = CellGraph();
    std::string input;
    std::ifstream file(filename);
    while (std::getline(file, input)) {
        graph.addCell((char*)input.c_str());
//        printf("you typed: %s\n", input.c_str());
    }
    graph.stopInitialBuilding();
    auto pool = CellThreadPool(graph.getQueue(), 10);
    pool.join();
//    for (auto &it : graph) {
//        if (it.second.getState() == DONE) {
//            std::cout << it.first << " = " << it.second.getResult() << std::endl;
//        } else {
//            std::cout << it.first << " = UNDEFINED" << std::endl;
//        }
//    }
}


int main(int argc, char* argv[]) {
    std::string name;
    if (argc <= 1) {
        std::cin >> name;
    } else {
        name.append(argv[1]);
    }
    test1(name);
    return 0;
}

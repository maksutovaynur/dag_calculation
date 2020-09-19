#include "fstream"
#include "iostream"
#include "string"
#include "parser.hpp"
#include "graph.hpp"
#include "mtime.h"


using namespace std;


void test1(string & inFileName, string & outFileName, int threadCount) {
    string input;
    ifstream inFile(inFileName);

    CGraph graph;

    while (getline(inFile, input)) {
        auto parsed = parse((char *)input.c_str());
        graph.addNode(parsed.name, parsed.operands, parsed.arg);
    }
    cout << "Built graph, size = " << graph.size() << endl;

    graph.preprocessOneThreaded();

    cout << "Topological sort performed" << endl;

    ptime pstart = now();

    graph.calculateOneThreaded();

    ptime pend = now();

    cout
        << "Calculation time using "
        << threadCount
        << " threads: "
        << ((float)durationCast(pend - pstart))/1000000
        << " s"
        << endl;
    ofstream outFile(outFileName);

    for (auto & c : graph ) {
        outFile << c.first << " = " << c.second.arg << endl;
    }
}


int main(int argc, char* argv[]) {
    string inFileName, outFileName;
    int threadCount;
    if (argc <= 1) {
        cin >> inFileName;
    } else {
        inFileName.append(argv[1]);
    }
    if (argc <= 2) {
        cin >> outFileName;
    } else {
        outFileName.append(argv[2]);
    }
    if (argc <= 3) {
        cin >> threadCount;
    } else {
        threadCount = stoi(argv[3]);
    }
    test1(inFileName, outFileName, threadCount);
    return 0;
}

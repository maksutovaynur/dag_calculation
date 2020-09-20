#ifndef DAG_CALCULATION_GRAPH_H
#define DAG_CALCULATION_GRAPH_H

#include "types.hpp"
#include "map"
#include "set"
#include "string"
#include "vector"
#include "parser.hpp"


using namespace std;


struct CNode {
    vector<CNode*> operands;
    vector<CNode*> forwardRefs;
    DataType arg = 0;
    CellType type = VALUE;
    size_t searchtime_state;
};


void recursiveDFS(CNode *node, vector<CNode*> & order) {
    if (node->searchtime_state == 1)
        throw "Cycle detected";
    else if (node->searchtime_state == 2)
        return;

    node->searchtime_state = 1;
    for (auto & c : node->operands)
        recursiveDFS(c, order);
    node->searchtime_state = 2;
    if (node->type == FORMULA)
        order.emplace_back(node);
}


void addOperand (CNode * operand, CNode * forwardRef) {
    forwardRef->operands.emplace_back(operand);
    forwardRef->type = FORMULA;
    operand->forwardRefs.emplace_back(forwardRef);
}


class CGraph {
public:
    void addNode (string & name, vector<string> & operands, DataType arg) {
        CNode & cell = nodes_heap[name];
        cell.arg = arg;
        CNode * forwardRef = &cell;
        preprocessingQueue.emplace_back(forwardRef);
        for (auto &c : operands) {
            CNode * operand = & nodes_heap[c];
            addOperand(operand, forwardRef);
            finishingNodes.erase(operand);
        }
    }
    size_t size() {
        return nodes_heap.size();
    }

    void preprocessOneThreaded () {
        if (!preprocessingQueue.empty()) preprocessNodes();
        buildTopologicalSort();
    }

    void calculateOneThreaded () {
        for (auto & c: sortedNodes) {
            auto data1 = 0;
            for (auto & operand : c->operands) {
                data1 += operand->arg;
            }
            c->arg = data1;
        }
    }

    auto begin() {
        return nodes_heap.begin();
    }

    auto end() {
        return nodes_heap.end();
    }

private:
    map<string, CNode> nodes_heap;
    set<CNode *> startingNodes;
    set<CNode *> finishingNodes;
    vector<CNode *> preprocessingQueue;
    vector<CNode *> sortedNodes;
    void preprocessNodes () {
        for (auto & c: preprocessingQueue) {
            if (c->operands.empty()) startingNodes.insert(c);
            else startingNodes.erase(c);
            if (c->forwardRefs.empty()) finishingNodes.insert(c);
            else finishingNodes.erase(c);
        }
    }
    void buildTopologicalSort() {
        sortedNodes.clear();
        for (auto & c : nodes_heap) {
            c.second.searchtime_state = 0;
        }
        for (auto & c : finishingNodes) {
            recursiveDFS(c, sortedNodes);
        }
    }
};



#endif

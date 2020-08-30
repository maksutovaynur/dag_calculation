//
// Created by aynur on 22.08.2020.
//

#ifndef DAG_CALCULATION_CGRAPH_H
#define DAG_CALCULATION_CGRAPH_H

#include "unordered_map"
#include "unordered_set"
#include "cell.hpp"
#include "parser.hpp"
#include "types.hpp"
#include "thread"


class CellGraph {
public:
    void addCell(char *c){
        ParsedCellInput input = parse(c); // parse input string
        std::vector<Cell *> childNodes;
        auto & currentNode = nodes[input.name]; // create or get node from map
        currentNode.writeLock();
        currentNode.name.clear();
        currentNode.name.append(input.name);
        for (auto & name: input.refs) {
            auto & childNode = nodes[name]; // create or get child node
            childNode.writeLock();              // lock all participating cells
            childNodes.push_back(&childNode);
        }
        currentNode.setRefs(childNodes);     // create refs between cells
        if (input.type == VALUE) currentNode.setArgument(input.arg);

        for (auto & childNode: childNodes) {
            childNode->writeUnlock();
        }

        currentNode.writeUnlock();

        if (! isInInitialBuilding()) {
            // On initial buildind, most efficient way is to select terminal nodes
            // after whole process of building;
            // when editing already existing graph,
            // ineffective recalculation on each node should be used
            queue.push(QueueTask{&currentNode, 1}); //CalcVersionHolder::incVersion()
        }
    }
    CellGraph () {
        isInitialBuilding = true;
    }
    void stopInitialBuilding () {
        initialBuildingMutex.lock();
        CalcVersion version = 1; //CalcVersionHolder::incVersion();
        isInitialBuilding = false;

        for (auto & it : nodes) {
            Cell & c = it.second;
            c.readLock();
            if (c.getType() == VALUE) {
                queue.push(QueueTask { &c, version });
            }
            c.readUnlock();
        }
        initialBuildingMutex.unlock();
    }
    bool isInInitialBuilding () {
        initialBuildingMutex.lock_shared();
        bool v = isInitialBuilding;
        initialBuildingMutex.unlock_shared();
        return v;
    }
    CellQueue *getQueue() {
        return &queue;
    }
    auto begin (){
        return nodes.begin();
    }
    auto end() {
        return nodes.end();
    }
private:
    std::unordered_map<std::string, Cell> nodes;
    std::unordered_set<Cell*> startingNodes;
    std::shared_mutex initialBuildingMutex;
    bool isInitialBuilding;
    CellQueue queue;
};

#endif //DAG_CALCULATION_CGRAPH_H

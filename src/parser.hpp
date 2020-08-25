//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_PARSER_H
#define DAG_CALCULATION_PARSER_H

#include "string"
#include "vector"
#include "types.hpp"


struct ParsedCellInput {
    std::string name;
    std::vector<std::string> refs;
    ArgType arg;
    CellType type;
};

void appendToString(std::string & str, char *c, int start, int end) {
    for (int j = start; j <= end; j ++) str.push_back(c[j]);
}

ParsedCellInput parse(char *c) {
    int lastPosition = 0, i;
    ParsedCellInput input;
    for (i = 0; true; i ++) {
        char cc = c[i];
        if (cc == '=') {
            appendToString(input.name, c, lastPosition, i - 2);
            lastPosition = i + 2;
        }
        else if (cc == '+' || cc == '\0') {
            input.refs.push_back(std::string(&c[lastPosition], i - lastPosition));
            lastPosition = i + 1;
            if (cc == '\0') break;
        }
    }
    if (input.refs.size() == 1) {
        input.arg = std::stoll(input.refs.back());
        input.refs.pop_back();
        input.type = VALUE;
    } else input.type = FORMULA;
    return std::move(input);
}


#endif //DAG_CALCULATION_PARSER_H

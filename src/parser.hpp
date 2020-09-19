//
// Created by aynur on 23.08.2020.
//

#ifndef DAG_CALCULATION_PARSER_H
#define DAG_CALCULATION_PARSER_H

#include "string"
#include "vector"
#include "types.hpp"
#include "optional"


struct ParsedCellInput {
    std::string name;
    std::vector<std::string> operands;
    DataType arg = 0;
    CellType type = VALUE;
};


void appendToString(std::string & str, char *c, int start, int end) {
    for (int j = start; j <= end; j ++) str.push_back(c[j]);
}


std::optional<DataType> stoa(std::string & str) {
    try {
        return std::optional<DataType>(std::stoll(str));
    } catch (std::exception &e) {
        return std::nullopt;
    }
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
            input.operands.emplace_back(std::string(&c[lastPosition], i - lastPosition));
            lastPosition = i + 1;
            if (cc == '\0') break;
        }
    }
    input.type = FORMULA;
    if (input.operands.size() == 1) {
        auto res = stoa(input.operands.back());
        if (res.has_value()) {
            input.arg = res.value();
            input.operands.pop_back();
            input.type = VALUE;
        }
    }
    return std::move(input);
}


#endif //DAG_CALCULATION_PARSER_H

//
// Created by moham on 07/06/2026.
//

#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <string>
#include <unordered_map>

#include "CommandFactories.h"
using namespace std;

class Parser {
    unordered_map<string,unique_ptr<CommandFactory>> factories;
public:
    Parser();

    unique_ptr<Command> parse(const string& line) const;
};
#endif //PARSER_H

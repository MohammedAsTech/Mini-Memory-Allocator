//
// Created by moham on 07/06/2026.
//

#include "Parser.h"

Parser::Parser() {
    factories["INIT"]  = std::make_unique<InitCommandFactory>();
    factories["ALLOC"] = std::make_unique<AllocCommandFactory>();
    factories["FREE"]  = std::make_unique<FreeCommandFactory>();
    factories["PRINT"] = std::make_unique<PrintCommandFactory>();
    factories["STATS"] = std::make_unique<StatsCommandFactory>();
    factories["STRATEGY"] = std::make_unique<StrategyCommandFactory>();
}

unique_ptr<Command> Parser::parse(const string &line) const {
    std::stringstream ss(line);

    std::string commandName;
    ss >> commandName;

    if (commandName.empty()) {
        throw std::invalid_argument("Empty command");
    }

    auto it = factories.find(commandName);

    if (it == factories.end()) {
        throw std::invalid_argument("Unknown command");
    }

    return it->second->create(ss);
}



//
// Created by moham on 07/06/2026.
//
#include "CommandFactories.h"

#include <stdexcept>
#include <string>

using std::invalid_argument;
using std::make_unique;
using std::string;
using std::unique_ptr;

unique_ptr<Command>
InitCommandFactory::create(std::stringstream& ss) const {
    int size;

    if (!(ss >> size)) {
        throw invalid_argument("INIT requires a size");
    }

    string extra;
    if (ss >> extra) {
        throw invalid_argument("INIT takes exactly one argument");
    }

    return make_unique<InitCommand>(size);
}

unique_ptr<Command>
AllocCommandFactory::create(std::stringstream& ss) const {
    int size;

    if (!(ss >> size)) {
        throw invalid_argument("ALLOC requires a size");
    }

    string extra;
    if (ss >> extra) {
        throw invalid_argument("ALLOC takes exactly one argument");
    }

    return make_unique<AllocCommand>(size);
}

unique_ptr<Command>
FreeCommandFactory::create(std::stringstream& ss) const {
    int id;

    if (!(ss >> id)) {
        throw invalid_argument("FREE requires an id");
    }

    string extra;
    if (ss >> extra) {
        throw invalid_argument("FREE takes exactly one argument");
    }

    return make_unique<FreeCommand>(id);
}

unique_ptr<Command>
PrintCommandFactory::create(std::stringstream& ss) const {
    string extra;

    if (ss >> extra) {
        throw invalid_argument("PRINT takes no arguments");
    }

    return make_unique<PrintCommand>();
}

unique_ptr<Command>
StatsCommandFactory::create(std::stringstream& ss) const {
    string extra;

    if (ss >> extra) {
        throw invalid_argument("STATS takes no arguments");
    }

    return make_unique<StatsCommand>();
}
std::unique_ptr<Command>
StrategyCommandFactory::create(std::stringstream& ss) const {
    std::string strategyName;

    if (!(ss >> strategyName)) {
        throw std::invalid_argument("STRATEGY requires strategy name");
    }

    std::string extra;
    if (ss >> extra) {
        throw std::invalid_argument("STRATEGY takes exactly one argument");
    }

    if (strategyName != "FIRST_FIT" && strategyName != "BEST_FIT") {
        throw std::invalid_argument("Unknown strategy");
    }

    return std::make_unique<StrategyCommand>(strategyName);
}
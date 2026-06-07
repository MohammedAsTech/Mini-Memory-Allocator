//
// Created by moham on 07/06/2026.
//

#ifndef COMMANDFACTORIES_H
#define COMMANDFACTORIES_H
#pragma once

#include <memory>
#include <sstream>
#include "Commands.h"

class CommandFactory {
public:
    virtual ~CommandFactory() = default;
    virtual std::unique_ptr<Command> create(std::stringstream& ss) const = 0;
};
class InitCommandFactory : public CommandFactory {
public:
    std::unique_ptr<Command>
    create(std::stringstream& ss) const override;
};

class AllocCommandFactory : public CommandFactory {
public:
    std::unique_ptr<Command>
    create(std::stringstream& ss) const override;
};

class FreeCommandFactory : public CommandFactory {
public:
    std::unique_ptr<Command>
    create(std::stringstream& ss) const override;
};

class PrintCommandFactory : public CommandFactory {
public:
    std::unique_ptr<Command>
    create(std::stringstream& ss) const override;
};

class StatsCommandFactory : public CommandFactory {
public:
    std::unique_ptr<Command>
    create(std::stringstream& ss) const override;
};
#endif //COMMANDFACTORIES_H

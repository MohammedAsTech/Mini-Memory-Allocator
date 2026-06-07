//
// Created by moham on 07/06/2026.
//

#ifndef COMMAND_H
#define COMMAND_H
#include <string>

#include "MemoryAllocator.h"
using namespace std;

class Command {
    string key;
public:
    Command(const string& key):key(key) {};
    virtual ~Command() = default;
    virtual void execute(MemoryAllocator& MA) = 0;
};
class InitCommand : public Command {
    int size;
public:
    InitCommand(int size):Command("INIT"),size(size) {};
    void execute(MemoryAllocator &MA) override;
};
class FreeCommand : public Command {
    int id;
public:
    FreeCommand(int id):Command("FREE"),id(id) {};
    void execute(MemoryAllocator &MA) override;
};
class AllocCommand : public Command {
    int size;
public:
    AllocCommand(int size):Command("ALLOC"),size(size) {};
    void execute(MemoryAllocator &MA) override;
};
class PrintCommand : public Command {

public:
    PrintCommand():Command("PRINT"){};
    void execute(MemoryAllocator &MA) override;
};
class StatsCommand : public Command {

public:
    StatsCommand():Command("STATS"){};
    void execute(MemoryAllocator &MA) override;
};
#endif //COMMAND_H

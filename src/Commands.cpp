//
// Created by moham on 07/06/2026.
//
#include "Commands.h"
#include <iostream>

void InitCommand::execute(MemoryAllocator& MA) {
    MA.init(size);
}

void AllocCommand::execute(MemoryAllocator& MA) {
    MA.allocate(size);
}

void FreeCommand::execute(MemoryAllocator& MA) {
    MA.free(id);
}

void PrintCommand::execute(MemoryAllocator& MA) {
    for (const auto& block : MA.getHeap()) {
        block.print();
        std::cout << " ";
    }

    std::cout << std::endl;
}

void StatsCommand::execute(MemoryAllocator& MA) {
    //MA.stats();
}
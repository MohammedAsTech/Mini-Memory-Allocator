//
// Created by moham on 07/06/2026.
//
#include "Commands.h"
#include <iostream>
#include "AllocationStrategy.h"
#include <memory>
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
    MA.stats();
}
void StrategyCommand::execute(MemoryAllocator& MA) {
    if (strategyName == "FIRST_FIT") {
        MA.setStrategy(std::make_unique<FirstFitStrategy>());
    }
    else if (strategyName == "BEST_FIT") {
        MA.setStrategy(std::make_unique<BestFitStrategy>());
    }
    else {
        throw std::invalid_argument("Unknown strategy");
    }
}
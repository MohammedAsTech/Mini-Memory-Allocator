//
// Created by moham on 07/06/2026.
//
#include "Statistics.h"

#include <algorithm>
#include <iostream>

int Statistics::findLargestFreeBlock(const std::list<MemoryBlock>& heap) {
    int largestFree = 0;

    for (const auto& block : heap) {
        if (block.getIsFree()) {
            largestFree = std::max(largestFree, block.getSize());
        }
    }

    return largestFree;
}

double Statistics::calculateFragmentation(int totalFree, int largestFree) {
    if (totalFree == 0) {
        return 0.0;
    }

    return 100.0 * (1.0 - static_cast<double>(largestFree) / totalFree);
}

void Statistics::printStats(
    const std::list<MemoryBlock>& heap,
    int totalFree
) {
    int largestFree = findLargestFreeBlock(heap);
    double fragmentation = calculateFragmentation(totalFree, largestFree);

    std::cout
        << "Total free: " << totalFree
        << " Largest free: " << largestFree
        << " Fragmentation: " << fragmentation << "%"
        << std::endl;
}
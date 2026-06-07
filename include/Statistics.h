//
// Created by moham on 07/06/2026.
//

#ifndef STATISTICS_H
#define STATISTICS_H
#pragma once

#include <list>
#include "MemoryBlock.h"

class Statistics {
public:
    static int findLargestFreeBlock(const std::list<MemoryBlock>& heap);

    static double calculateFragmentation(
        int totalFree,
        int largestFree
    );

    static void printStats(
        const std::list<MemoryBlock>& heap,
        int totalFree
    );
};
#endif //STATISTICS_H

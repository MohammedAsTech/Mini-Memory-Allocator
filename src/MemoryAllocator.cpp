//
// Created by moham on 06/06/2026.
//
#include "MemoryAllocator.h"

#include <stdexcept>

void MemoryAllocator::init(int size) {
    if(size <= 0)
        throw invalid_argument("size must be positive");
    heap.clear();
    nextIndex = 1;
    heap.emplace_back(nextIndex,0,size,true);
    // emplace_back() creates an object directly inside the list without creating a temporary object first.
}


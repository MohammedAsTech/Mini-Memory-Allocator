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
    heap.emplace_back(-1,0,size,true);
    // emplace_back() creates an object directly inside the list without creating a temporary object first.
}
void MemoryAllocator::allocate(int size) {
    if (size <= 0) {
        throw std::invalid_argument("Allocation size must be positive");
    }
    for (auto it = heap.begin(); it != heap.end(); ++it) {
        if (it->getIsFree() && it->getSize() >= size) {

            int oldStart = it->getStart();

            if (it->getSize() == size) {
                it->SetIsFree(false);
                it->setId(nextIndex++);
                return;
            }

            // split block
            heap.insert(
                it,
                MemoryBlock(nextIndex++, oldStart, size, false)
            );

            it->setStart(oldStart + size);
            it->setSize(it->getSize() - size);

            return;
        }
    }

    throw std::runtime_error("Not enough memory");
}


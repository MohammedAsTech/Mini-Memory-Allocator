#include "AllocationStrategy.h"
#include "MemoryAllocator.h"
#include <stdexcept>
//
// Created by moham on 07/06/2026.
//
void FirstFitStrategy::allocate(MemoryAllocator& a, int size) {
    if (size <= 0) {
        throw std::invalid_argument("Allocation size must be positive");
    }

    for (auto it = a.heap.begin(); it != a.heap.end(); ++it) {
        if (it->getIsFree() && it->getSize() >= size) {

            int oldStart = it->getStart();
            int oldSize = it->getSize();

            if (oldSize == size) {
                it->SetIsFree(false);
                it->setId(a.nextIndex++);
                a.totalfree -= size;
                return;
            }

            // Split block
            a.heap.insert(
                it,
                MemoryBlock(a.nextIndex++, oldStart, size, false)
            );

            it->setStart(oldStart + size);
            it->setSize(oldSize - size);

            a.totalfree -= size;
            return;
        }
    }

    throw std::runtime_error("Not enough memory");
}
void BestFitStrategy::allocate(MemoryAllocator& a, int size) {
    if (size <= 0) {
        throw std::invalid_argument("Allocation size must be positive");
    }

    auto best = a.heap.end();

    for (auto it = a.heap.begin(); it != a.heap.end(); ++it) {
        if (it->getIsFree() && it->getSize() >= size) {
            if (best == a.heap.end() ||
                it->getSize() < best->getSize()) {
                best = it;
                }
        }
    }

    if (best == a.heap.end()) {
        throw std::runtime_error("Not enough memory");
    }

    int oldStart = best->getStart();
    int oldSize = best->getSize();

    if (oldSize == size) {
        best->SetIsFree(false);
        best->setId(a.nextIndex++);
        a.totalfree -= size;
        return;
    }

    a.heap.insert(
        best,
        MemoryBlock(a.nextIndex++, oldStart, size, false)
    );

    best->setStart(oldStart + size);
    best->setSize(oldSize - size);

    a.totalfree -= size;
}


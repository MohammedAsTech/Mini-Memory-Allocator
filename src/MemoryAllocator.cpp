//
// Created by moham on 06/06/2026.
//
#include "MemoryAllocator.h"

#include <stdexcept>
using namespace std;

void MemoryAllocator::init(int size) {
    if(size <= 0)
        throw invalid_argument("size must be positive");
    heap.clear();
    nextIndex = 1;
    totalfree = size;
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
                totalfree -= size;
                return;
            }

            // split block
            heap.insert(
                it,
                MemoryBlock(nextIndex++, oldStart, size, false)
            );

            it->setStart(oldStart + size);
            it->setSize(it->getSize() - size);
            totalfree -= size;
            return;
        }
    }

    throw std::runtime_error("Not enough memory");
}
void MemoryAllocator::free(int id) {
    for(auto it = heap.begin();it != heap.end();++it) {
        if(it->getId() == id) {
            if(it->getIsFree())
                throw invalid_argument("id already free!");
            it->setId(-1);
            it->SetIsFree(true);
            totalfree += it->getSize();
            mergeIfNeeded(it);
            return;
        }
    }
    throw invalid_argument("id does not exist!");
}
void MemoryAllocator::mergeIfNeeded(std::list<MemoryBlock>::iterator curr) {
    if (curr == heap.end() || !curr->getIsFree()) {
        return;
    }

    // Merge with previous free block
    if (curr != heap.begin()) {
        auto prev = std::prev(curr);

        if (prev->getIsFree()) {
            prev->setSize(prev->getSize() + curr->getSize());

            heap.erase(curr);
            curr = prev;
        }
    }

    // Merge with next free block
    auto next = std::next(curr);

    if (next != heap.end() && next->getIsFree()) {
        curr->setSize(curr->getSize() + next->getSize());

        heap.erase(next);
    }
}

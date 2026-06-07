//
// Created by moham on 06/06/2026.
//

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <list>
#include <memory>
#include <MemoryBlock.h>

#include "AllocationStrategy.h"
using namespace std;


class MemoryAllocator {
private:
    list<MemoryBlock> heap;
    int nextIndex;
    void mergeIfNeeded(std::list<MemoryBlock>::iterator curr);
    unique_ptr<AllocationStrategy> stra;
    int totalfree;
    friend class FirstFitStrategy;
    friend class BestFitStrategy;
public:

    MemoryAllocator()
    : nextIndex(1),
      stra(make_unique<FirstFitStrategy>()),
      totalfree(0) {}

    void init(int size);

    void allocate(int size);

    void free(int id);

    list<MemoryBlock>& getHeap(){return heap;}

    const list<MemoryBlock>& getHeap() const{return heap;}

    void setStrategy(std::unique_ptr<AllocationStrategy> newStrategy);

    void stats() const;


};

#endif //MEMORYALLOCATOR_H

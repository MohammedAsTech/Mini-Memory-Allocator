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
    int totalfree;
    unique_ptr<AllocationStrategy> stra;

public:
    MemoryAllocator():nextIndex(1),totalfree(0){}

    void init(int size);

    void allocate(int size);

    void free(int id);

    list<MemoryBlock>& getHeap(){return heap;}

    const list<MemoryBlock>& getHeap() const{return heap;}
};

#endif //MEMORYALLOCATOR_H

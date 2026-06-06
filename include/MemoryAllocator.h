//
// Created by moham on 06/06/2026.
//

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H
#include <list>
#include <MemoryBlock.h>
using namespace std;


class MemoryAllocator {
private:
    list<MemoryBlock> heap;
    int nextIndex;
    void mergeIfNeeded(std::list<MemoryBlock>::iterator curr);

public:
    MemoryAllocator():nextIndex(1){}

    void init(int size);

    void allocate(int size);

    void free(int id);
};

#endif //MEMORYALLOCATOR_H

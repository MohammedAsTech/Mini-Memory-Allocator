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
public:
    MemoryAllocator():nextIndex(1){}

    void init(int size);

    void allocate(int size);
};

#endif //MEMORYALLOCATOR_H

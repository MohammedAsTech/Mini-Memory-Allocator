#include "MemoryBlock.h"
#include "iostream"
//
// Created by moham on 05/06/20
int MemoryBlock::getSize() const {
    return size;
}
int MemoryBlock::getId() const {
    return id;
}
int MemoryBlock::getStart() const {
    return start;
}
void MemoryBlock::setId(int val) {
    id = val;
}
void MemoryBlock::setSize(int val) {
    size = val;
}
void MemoryBlock::setStart(int val) {
    start = val;
}
void MemoryBlock::SetIsFree(bool val) {
    isFree = val;
}
bool MemoryBlock::getIsFree() const {
    return isFree;
}
void MemoryBlock::print() const {
    if (isFree) {
        std::cout << "[free start=" << start
                  << " size=" << size << "]";
    } else {
        std::cout << "[id=" << id
                  << " start=" << start
                  << " size=" << size << "]";
    }
}







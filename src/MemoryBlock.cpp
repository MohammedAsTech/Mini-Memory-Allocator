#include "MemoryBlock.h"
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







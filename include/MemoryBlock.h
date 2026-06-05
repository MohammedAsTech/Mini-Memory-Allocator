//
// Created by moham on 05/06/2026.
//

#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H
class MemoryBlock {
private:
    int id;
    int start;
    int size;
    bool isFree;
public:
    MemoryBlock(int id,int start,int size,bool isFree):id(id),start(start)
    ,size(size),isFree(isFree){}

    ~MemoryBlock() = default;

    int getId() const;

    int getStart() const;

    int getSize() const;

    bool getIsFree() const;

    void setId(int val);

    void setSize(int val);

    void setStart(int val);

    void SetIsFree(bool val);

    void print() const;


};

#endif //MEMORYBLOCK_H

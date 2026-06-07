#ifndef ALLOCATIONSTRATEGY_H
#define ALLOCATIONSTRATEGY_H

class MemoryAllocator;

class AllocationStrategy {
public:
  virtual ~AllocationStrategy() = default;

  virtual void allocate(
      MemoryAllocator& allocator,
      int size
  ) = 0;
};

class FirstFitStrategy : public AllocationStrategy {
public:
  void allocate(
      MemoryAllocator& allocator,
      int size
  ) override;
};

class BestFitStrategy : public AllocationStrategy {
public:
  void allocate(
      MemoryAllocator& allocator,
      int size
  ) override;
};

#endif // ALLOCATIONSTRATEGY_H
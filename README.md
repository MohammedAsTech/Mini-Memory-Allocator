# Mini Memory Allocator

A C++ implementation of a simplified memory manager that simulates how operating systems and low-level runtime environments allocate and free memory.

The project manages a contiguous block of memory, supports multiple allocation strategies, handles fragmentation, merges adjacent free blocks, and provides runtime statistics about memory usage.

---

## Overview

Memory allocation is one of the core responsibilities of an operating system and runtime environment. When a program requests memory, the allocator must locate a suitable free region, reserve it, and later return it to the available memory pool when the memory is released.

This project simulates that process by maintaining a list of memory blocks. Each block stores:

* Block ID
* Start address
* Size
* Allocation status (free/allocated)

When memory is allocated, the allocator searches for a suitable free block and may split it into smaller blocks. When memory is freed, adjacent free blocks are automatically merged to reduce fragmentation and improve future allocation efficiency.

The goal of this project is to demonstrate systems programming concepts, memory management techniques, object-oriented design, and modern C++ development practices.

---

## Features

* Dynamic memory allocation
* Memory deallocation
* Automatic block splitting
* Automatic block merging (coalescing)
* Runtime memory statistics
* Multiple allocation strategies
* Command-based architecture
* Input parsing and validation
* Exception-based error handling

---

## Allocation Strategies

### First Fit

Allocates memory in the first free block large enough to satisfy the request.

**Advantages**

* Fast allocation
* Low overhead

**Disadvantages**

* Can create fragmentation near the beginning of memory

### Best Fit

Allocates memory in the smallest free block that can satisfy the request.

**Advantages**

* Reduces wasted space per allocation

**Disadvantages**

* Requires scanning more blocks
* Can create many small unusable fragments

The allocation strategy can be changed at runtime without modifying the allocator itself.

---

## Supported Commands

| Command              | Description                       |
| -------------------- | --------------------------------- |
| `INIT <size>`        | Initialize the memory pool        |
| `ALLOC <size>`       | Allocate memory                   |
| `FREE <id>`          | Release a block                   |
| `PRINT`              | Display the current memory layout |
| `STATS`              | Display memory statistics         |
| `STRATEGY FIRST_FIT` | Switch to First Fit allocation    |
| `STRATEGY BEST_FIT`  | Switch to Best Fit allocation     |

### Example

```text
INIT 100
ALLOC 30
ALLOC 20
FREE 1
PRINT
STATS
```

---

## Design

The project is divided into independent components, each responsible for a specific part of the system:

* `MemoryAllocator` – manages allocation, deallocation, and memory state
* `MemoryBlock` – represents a memory segment
* `Parser` – converts user input into executable commands
* `Command` hierarchy – encapsulates user operations
* `CommandFactory` hierarchy – creates commands from parsed input
* `AllocationStrategy` hierarchy – implements allocation algorithms
* `Statistics` – calculates fragmentation and memory metrics

This separation keeps the code modular, maintainable, and easy to extend.

---

## Design Patterns

### Command Pattern

Each user instruction is represented by its own command class (`InitCommand`, `AllocCommand`, `FreeCommand`, `PrintCommand`, etc.).

Instead of relying on large `if`/`switch` statements, every operation is encapsulated in an object responsible for executing its own behavior.

**Benefits**

* Easy to add new commands
* Cleaner code organization
* Better scalability and maintainability

---

### Factory Pattern

Command creation is delegated to specialized factory classes (`InitCommandFactory`, `AllocCommandFactory`, `FreeCommandFactory`, etc.).

The parser identifies the command type and forwards the remaining input to the appropriate factory, which validates the arguments and creates the correct command object.

**Benefits**

* Separates parsing from object creation
* Simplifies command registration
* Improves maintainability

---

### Strategy Pattern

Different allocation algorithms are implemented as interchangeable strategies.

The allocator stores an `AllocationStrategy` object and can switch between:

* `FirstFitStrategy`
* `BestFitStrategy`

at runtime without changing the allocator implementation.

**Benefits**

* Open for extension without modifying existing code
* Easy to add new allocation algorithms
* Demonstrates runtime polymorphism

---

### Why These Patterns?

Memory allocators frequently evolve by introducing new allocation algorithms and management features.

Using Command, Factory, and Strategy patterns keeps the system modular, extensible, and aligned with object-oriented design principles while avoiding tightly coupled code.

---

## Complexity Analysis

| Operation              | Complexity |
| ---------------------- | ---------- |
| Allocate (First Fit)   | O(n)       |
| Allocate (Best Fit)    | O(n)       |
| Free                   | O(n)       |
| Merge Adjacent Blocks  | O(1)       |
| Print Memory Layout    | O(n)       |
| Statistics Calculation | O(n)       |

Where `n` is the number of memory blocks currently managed by the allocator.

---

## Testing

The project was validated using:

* 300 functional test cases
* Compilation checks with strict warning flags
* AddressSanitizer (ASAN)
* UndefinedBehaviorSanitizer (UBSAN)
* Memory leak detection
* Fragmentation validation tests
* Block splitting and merging tests
* Allocation strategy comparison tests

---

## Technologies

* C++20
* STL (`list`, `unique_ptr`, `unordered_map`)
* Object-Oriented Programming
* Design Patterns
* CMake
* AddressSanitizer
* UndefinedBehaviorSanitizer

---

## Learning Outcomes

This project demonstrates practical experience with:

* Dynamic memory management
* Memory fragmentation and coalescing
* Allocation algorithms
* Systems programming concepts
* Modern C++
* Design patterns
* Software architecture
* Testing and debugging methodologies
* Runtime polymorphism and abstraction


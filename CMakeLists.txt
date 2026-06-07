cmake_minimum_required(VERSION 3.29)

project(MiniMemoryAllocator)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(MiniMemoryAllocator
        src/main.cpp
        src/MemoryBlock.cpp
        include/MemoryAllocator.h
        src/MemoryAllocator.cpp
        include/Statistics.h
        src/Statistics.cpp
        include/Commands.h
        src/Commands.cpp
        include/CommandFactories.h
        src/CommandFactories.cpp
        include/Parser.h
        src/Parser.cpp
)
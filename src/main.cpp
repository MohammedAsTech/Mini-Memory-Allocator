#include <iostream>
#include <string>

#include "Parser.h"
#include "MemoryAllocator.h"

int main() {
    MemoryAllocator allocator;
    Parser parser;

    std::string line;

    while (std::getline(std::cin, line)) {
        try {
            if (line == "EXIT") {
                std::cout << "Goodbye" << std::endl;
                break;
            }

            auto command = parser.parse(line);
            command->execute(allocator);
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}
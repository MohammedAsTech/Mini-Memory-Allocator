#include <iostream>
#include <string>

int main() {
    bool initialized = false;
    std::string input;

    while (std::getline(std::cin, input)) {

        if (input == "EXIT") {
            std::cout << "Goodbye" << std::endl;
            break;
        }

        else if (input == "INIT") {
            initialized = true;
            std::cout << "Heap initialized" << std::endl;
        }

        else if (input == "PRINT") {
            if (!initialized) {
                std::cout << "Heap not initialized" << std::endl;
            }
            else {
                std::cout << "Heap contents" << std::endl;
            }
        }

        else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}
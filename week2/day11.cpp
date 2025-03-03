#include <iostream>
#include <memory>
#include <fstream>

// Function to demonstrate unique_ptr with dynamic array
void uniquePtrExample() {
    std::unique_ptr<int[]> uniqueArray(new int[5]);
    for (int i = 0; i < 5; ++i) {
        uniqueArray[i] = i * 10;
    }
    for (int i = 0; i < 5; ++i) {
        std::cout << uniqueArray[i] << " ";
    }
    std::cout << std::endl;
}

// Function to demonstrate shared_ptr with a shared resource (log file)
void sharedPtrExample() {
    auto logFile = std::make_shared<std::ofstream>("log.txt", std::ios::app);
    if (!logFile->is_open()) {
        std::cerr << "Failed to open log file" << std::endl;
        return;
    }

    auto logWriter1 = logFile;
    auto logWriter2 = logFile;

    *logWriter1 << "Log entry from writer 1" << std::endl;
    *logWriter2 << "Log entry from writer 2" << std::endl;
}

int main() {
    uniquePtrExample();
    sharedPtrExample();
    return 0;
}
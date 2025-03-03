#include <iostream>
#include <vector>
#include <chrono>

class BigObject {
public:
    BigObject(size_t size) : data(size) {
        std::cout << "Constructing BigObject of size " << size << std::endl;
    }

    // Copy constructor
    BigObject(const BigObject& other) : data(other.data) {
        std::cout << "Copy constructing BigObject" << std::endl;
    }

    // Move constructor
    BigObject(BigObject&& other) noexcept : data(std::move(other.data)) {
        std::cout << "Move constructing BigObject" << std::endl;
    }

private:
    std::vector<int> data;
};

int main() {
    const size_t size = 10000000;

    // Measure copy constructor performance
    auto start = std::chrono::high_resolution_clock::now();
    BigObject obj1(size);
    BigObject obj2 = obj1; // Copy constructor
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> copy_duration = end - start;
    std::cout << "Copy constructor took " << copy_duration.count() << " seconds" << std::endl;

    // Measure move constructor performance
    start = std::chrono::high_resolution_clock::now();
    BigObject obj3(size);
    BigObject obj4 = std::move(obj3); // Move constructor
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> move_duration = end - start;
    std::cout << "Move constructor took " << move_duration.count() << " seconds" << std::endl;

    return 0;
}
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

const int NUM_THREADS = 10;
const int INCREMENTS_PER_THREAD = 1000;

int counter = 0;
std::mutex counter_mutex;

void increment_counter() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        std::lock_guard<std::mutex> lock(counter_mutex);
        ++counter;
    }
}

int main() {
    std::vector<std::thread> threads;

    // Create and start threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(increment_counter);
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Final counter value: " << counter << std::endl;

    return 0;
}
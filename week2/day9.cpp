#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

// Function to convert a string to uppercase
std::string to_upper(const std::string& str) {
    std::string result;
    std::transform(str.begin(), str.end(), std::back_inserter(result), ::toupper);
    return result;
}

int main() {

    std::vector<std::string> vec {"apple", "banana", "cherry", "date"};

    // Sort the vector by string length using a lambda function
    sort(vec.begin(), vec.end(), [](const std::string& a, const std::string& b) {
        return a.size() < b.size();
    });

    // Transform each element to uppercase
    std::transform(vec.begin(), vec.end(), vec.begin(), [](std::string& str) {
        return to_upper(str);
    });

    // Print the sorted and transformed vector
    for (const auto& str : vec) {
        std::cout << str << std::endl;
    }

    return 0;
}
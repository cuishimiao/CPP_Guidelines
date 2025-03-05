#include <iostream>
#include <map>
#include <optional>
#include <string>

// Function that may fail and return an optional value
std::optional<std::string> findValue(const std::map<int, std::string>& myMap, int key) {
    auto it = myMap.find(key);
    if (it != myMap.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}

int main() {
    std::map<int, std::string> myMap = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    // Using structured bindings to iterate over the map
    for (const auto& [key, value] : myMap) {
        std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }

    // Example usage of the function that returns an optional value
    int keyToFind = 2;
    std::optional<std::string> result = findValue(myMap, keyToFind);

    if (result) {
        std::cout << "Found value: " << *result << std::endl;
    } else {
        std::cout << "Value not found for key: " << keyToFind << std::endl;
    }

    return 0;
}
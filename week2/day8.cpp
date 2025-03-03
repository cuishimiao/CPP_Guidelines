#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

int main() {
    std::ifstream file("textfile.txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    std::map<std::string, int> wordCount;
    std::string line, word;

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        while (stream >> word) {
            ++wordCount[word];
        }
    }

    file.close();

    for (const auto& pair : wordCount) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
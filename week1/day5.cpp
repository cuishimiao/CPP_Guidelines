#include <iostream>
#include <cstring>

class String {
private:
    char* data;
    size_t length;

public:
    // Default constructor
    String() : data(nullptr), length(0) {}

    // Constructor with C-string
    String(const char* str) {
        if (str) {
            length = std::strlen(str);
            data = new char[length + 1];
            std::strcpy(data, str);
        } else {
            data = nullptr;
            length = 0;
        }
    }

    // Copy constructor (deep copy)
    String(const String& other) {
        length = other.length;
        if (other.data) {
            data = new char[length + 1];
            std::strcpy(data, other.data);
        } else {
            data = nullptr;
        }
    }

    // Copy assignment operator (deep copy)
    String& operator=(const String& other) {
        if (this == &other) {
            return *this;
        }

        delete[] data;

        length = other.length;
        if (other.data) {
            data = new char[length + 1];
            std::strcpy(data, other.data);
        } else {
            data = nullptr;
        }

        return *this;
    }

    // Destructor
    ~String() {
        delete[] data;
    }

    // Get the length of the string
    size_t size() const {
        return length;
    }

    // Get the C-string
    const char* c_str() const {
        return data;
    }
};

int main() {
    String str1("Hello, World!");
    String str2 = str1; // Copy constructor
    String str3;
    str3 = str1; // Copy assignment operator

    std::cout << "str1: " << str1.c_str() << std::endl;
    std::cout << "str2: " << str2.c_str() << std::endl;
    std::cout << "str3: " << str3.c_str() << std::endl;

    return 0;
}
#include <iostream>

// Generic swap function template
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Template class Box
template <typename T>
class Box {
public:
    Box(T value) : value(value) {}

    T getValue() const {
        return value;
    }

    void setValue(T value) {
        this->value = value;
    }

private:
    T value;
};

int main() {
    // Test swap function
    int x = 10, y = 20;
    std::cout << "Before swap: x = " << x << ", y = " << y << std::endl;
    swap(x, y);
    std::cout << "After swap: x = " << x << ", y = " << y << std::endl;

    // Test Box class
    Box<int> intBox(123);
    std::cout << "Box value: " << intBox.getValue() << std::endl;

    Box<std::string> strBox("Hello");
    std::cout << "Box value: " << strBox.getValue() << std::endl;

    return 0;
}
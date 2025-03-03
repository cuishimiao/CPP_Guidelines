#include <iostream>

// Function to swap two integers using pointers
void swapUsingPointers(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to swap two integers using references
void swapUsingReferences(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 10, y = 20;

    std::cout << "Before swapping using pointers: x = " << x << ", y = " << y << std::endl;
    swapUsingPointers(&x, &y);
    std::cout << "After swapping using pointers: x = " << x << ", y = " << y << std::endl;

    std::cout << "Before swapping using references: x = " << x << ", y = " << y << std::endl;
    swapUsingReferences(x, y);
    std::cout << "After swapping using references: x = " << x << ", y = " << y << std::endl;

    return 0;
}
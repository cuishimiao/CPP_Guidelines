#include <iostream>
#include <stdexcept>
#include <limits>

double divide(double a, double b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero error");
    }
    return a / b;
}

int main() {
    double num1, num2;
    char op;

    try {
        std::cout << "Enter first number: ";
        if (!(std::cin >> num1)) {
            throw std::invalid_argument("Invalid input, not a number");
        }

        std::cout << "Enter an operator (+, -, *, /): ";
        std::cin >> op;

        std::cout << "Enter second number: ";
        if (!(std::cin >> num2)) {
            throw std::invalid_argument("Invalid input, not a number");
        }

        double result;
        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                result = divide(num1, num2);
                break;
            default:
                throw std::invalid_argument("Invalid operator");
        }

        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
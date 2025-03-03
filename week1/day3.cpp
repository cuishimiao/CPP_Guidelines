#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int age;

public:
    // Constructor
    Student(const std::string& name, int age) : name(name), age(age) {
        std::cout << "Constructor called for " << this->name << std::endl;
    }

    // Destructor
    ~Student() {
        std::cout << "Destructor called for " << this->name << std::endl;
    }

    // Method to print student information
    void printInfo() const {
        std::cout << "Name: " << this->name << ", Age: " << this->age << std::endl;
    }

    // Method to modify the age
    void setAge(int newAge) {
        this->age = newAge;
    }
};

int main() {
    // Create a Student object
    Student student("John Doe", 20);

    // Print student information
    student.printInfo();

    // Modify the age
    student.setAge(21);

    // Print student information again
    student.printInfo();

    return 0;
}
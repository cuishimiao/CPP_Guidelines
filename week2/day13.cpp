#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Student {
public:
    std::string name;
    int age;
    std::string id;

    Student() = default;
    Student(const std::string& name, int age, const std::string& id)
        : name(name), age(age), id(id) {}

    void serialize(std::ofstream& ofs) const {
        ofs << name << '\n' << age << '\n' << id << '\n';
    }

    void deserialize(std::ifstream& ifs) {
        std::getline(ifs, name);
        ifs >> age;
        ifs.ignore(); // Ignore the newline character after age
        std::getline(ifs, id);
    }
};

void saveStudents(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }
    for (const auto& student : students) {
        student.serialize(ofs);
    }
}

std::vector<Student> loadStudents(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return students;
    }
    while (ifs.peek() != EOF) {
        Student student;
        student.deserialize(ifs);
        students.push_back(student);
    }
    return students;
}

int main() {
    std::vector<Student> students = {
        {"Alice", 20, "S001"},
        {"Bob", 21, "S002"},
        {"Charlie", 22, "S003"}
    };

    std::string filename = "students.txt";

    // Save students to file
    saveStudents(students, filename);

    // Load students from file
    std::vector<Student> loadedStudents = loadStudents(filename);

    // Display loaded students
    for (const auto& student : loadedStudents) {
        std::cout << "Name: " << student.name << ", Age: " << student.age << ", ID: " << student.id << std::endl;
    }

    return 0;
}
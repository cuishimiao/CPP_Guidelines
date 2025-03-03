#include <iostream>
#include <cmath>

class Shape {
public:
    virtual double area() const = 0; // Pure virtual function
    virtual ~Shape() = default; // Virtual destructor
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    double area() const override {
        return M_PI * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override {
        return width * height;
    }
};

int main() {
    Shape* shapes[2];
    shapes[0] = new Circle(5.0);
    shapes[1] = new Rectangle(4.0, 6.0);

    for (int i = 0; i < 2; ++i) {
        std::cout << "Area of shape " << i + 1 << ": " << shapes[i]->area() << std::endl;
        delete shapes[i];
    }

    return 0;
}
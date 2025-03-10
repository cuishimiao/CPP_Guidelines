#include <iostream>
#include <fstream>
#include <mutex>
#include <memory>
#include <string>

// Thread-safe Singleton Logger class
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        logfile_ << message << std::endl;
    }

private:
    Logger() : logfile_("log.txt", std::ios::app) {
        if (!logfile_.is_open()) {
            throw std::runtime_error("Unable to open log file");
        }
    }

    ~Logger() {
        if (logfile_.is_open()) {
            logfile_.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logfile_;
    std::mutex mutex_;
};

// Shape interface
class Shape {
public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

// Circle class
class Circle : public Shape {
public:
    void draw() const override {
        Logger::getInstance().log("Drawing Circle");
        std::cout << "Circle drawn" << std::endl;
    }
};

// Square class
class Square : public Shape {
public:
    void draw() const override {
        Logger::getInstance().log("Drawing Square");
        std::cout << "Square drawn" << std::endl;
    }
};

// ShapeFactory class
class ShapeFactory {
public:
    enum ShapeType {
        CIRCLE,
        SQUARE
    };

    static std::unique_ptr<Shape> createShape(ShapeType type) {
        switch (type) {
            case CIRCLE:
                return std::make_unique<Circle>();
            case SQUARE:
                return std::make_unique<Square>();
            default:
                throw std::invalid_argument("Unknown shape type");
        }
    }
};

int main() {
    auto circle = ShapeFactory::createShape(ShapeFactory::CIRCLE);
    circle->draw();

    auto square = ShapeFactory::createShape(ShapeFactory::SQUARE);
    square->draw();

    return 0;
}
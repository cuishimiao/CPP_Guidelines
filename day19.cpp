#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Observer Pattern
class Observer {
public:
    virtual void update(const std::string &message) = 0;
};

class NewsSubscriber : public Observer {
private:
    std::string name;
public:
    NewsSubscriber(const std::string &name) : name(name) {}
    void update(const std::string &message) override {
        std::cout << name << " received: " << message << std::endl;
    }
};

class NewsPublisher {
private:
    std::vector<Observer *> subscribers;
public:
    void subscribe(Observer *subscriber) {
        subscribers.push_back(subscriber);
    }
    void unsubscribe(Observer *subscriber) {
        subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), subscriber), subscribers.end());
    }
    void notify(const std::string &message) {
        for (Observer *subscriber : subscribers) {
            subscriber->update(message);
        }
    }
};

// Strategy Pattern
class SortStrategy {
public:
    virtual void sort(std::vector<int> &data) = 0;
};

class QuickSort : public SortStrategy {
public:
    void sort(std::vector<int> &data) override {
        std::sort(data.begin(), data.end());
    }
};

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int> &data) override {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};

class SortContext {
private:
    SortStrategy *strategy;
public:
    SortContext(SortStrategy *strategy) : strategy(strategy) {}
    void setStrategy(SortStrategy *newStrategy) {
        strategy = newStrategy;
    }
    void sort(std::vector<int> &data) {
        strategy->sort(data);
    }
};

int main() {
    // Observer Pattern Demo
    NewsPublisher publisher;
    NewsSubscriber subscriber1("Alice");
    NewsSubscriber subscriber2("Bob");

    publisher.subscribe(&subscriber1);
    publisher.subscribe(&subscriber2);

    publisher.notify("Breaking News: Observer Pattern Implemented!");

    // Strategy Pattern Demo
    std::vector<int> data = {5, 3, 8, 6, 2, 7, 4, 1};

    SortContext context(new QuickSort());
    context.sort(data);

    std::cout << "QuickSort: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    context.setStrategy(new BubbleSort());
    context.sort(data);

    std::cout << "BubbleSort: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
#include <iostream>
#include <functional>
#include <vector>

class EventHandler {
public:
    using Callback = std::function<void()>;

    void registerCallback(Callback cb) {
        callbacks.push_back(cb);
    }

    void triggerEvent() {
        for (const auto& cb : callbacks) {
            cb();
        }
    }

private:
    std::vector<Callback> callbacks;
};

int main() {
    EventHandler handler;

    handler.registerCallback([]() {
        std::cout << "Callback 1 triggered!" << std::endl;
    });

    handler.registerCallback([]() {
        std::cout << "Callback 2 triggered!" << std::endl;
    });

    handler.triggerEvent();

    return 0;
}
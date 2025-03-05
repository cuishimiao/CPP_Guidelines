#include <iostream>
#include <vector>
#include <numeric>

template <typename Container>
auto sum(const Container& container) -> decltype(*container.begin()) {
    using ValueType = decltype(*container.begin());
    return std::accumulate(container.begin(), container.end(), ValueType{});
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::cout << "Sum of vector elements: " << sum(vec) << std::endl;

    std::vector<double> vec_d = {1.1, 2.2, 3.3, 4.4, 5.5};
    std::cout << "Sum of vector<double> elements: " << sum(vec_d) << std::endl;

    return 0;
}
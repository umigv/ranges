#include "iterator_range.hpp"
#include "mapped_range.hpp"
#include "range.hpp"

#include <vector>
#include <iostream>

using namespace umigv::ranges;

int int_double(int x) {
    return x * 2;
}

int main() {
    const std::vector<int> v{ 0, 1, 2, 3, 4, 5 };

    for (auto &&elem : iterator_range(v).map(int_double)) {
        std::cout << std::forward<decltype(elem)>(elem) << '\n';
    }
}

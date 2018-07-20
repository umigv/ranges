#include "ranges.hpp"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

struct PairMaker {
    template <typename T, typename U>
    constexpr decltype(auto) operator()(T &&x, U &&y) const {
        return std::make_pair(std::forward<T>(x), std::forward<U>(y));
    }
};

TEST(ZippedRange, Basic) {
    const std::vector<int> integers{ 0, 1, 2 };
    const std::vector<double> reals{ 3.14, 2.718 };

    const std::vector<std::pair<int, double>> zipped =
        umigv::ranges::adapt(integers)
            .zip(reals)
            .map(PairMaker{ })
            .collect();

    const std::vector<std::pair<int, double>> expected{
        { 0, 3.14 }, { 1, 2.718 }
    };

    EXPECT_TRUE(std::equal(zipped.cbegin(), zipped.cend(), expected.cbegin())
                && zipped.size() == expected.size());
}

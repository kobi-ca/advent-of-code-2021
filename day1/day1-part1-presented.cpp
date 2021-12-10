//
// Created by kobic on 12/7/21.
//

#include "range/v3/all.hpp"

#include "gtest/gtest.h"

/**
 * We are given a sequence of numbers (positive integers).
 * Our goal is to count the number of cases when a number in the series is higher than the preceding one.
 * So for example 0 0 0 0 0 the correct response would be 0 for 1 2 3 4 5 it would be 4 .
 */

namespace {

    uint32_t ranges_sol(std::istream& is) {
        return ranges::count_if(
                ranges::istream_view<uint32_t>(is),
                [prev = std::numeric_limits<uint32_t>::max()](const uint32_t curr) mutable{
                    return std::exchange(prev, curr) < curr;
                }
                );
    }

    uint32_t count_inc(const std::vector<uint32_t>& input) {
        // op()(...) const
        auto pred([prev = std::numeric_limits<uint32_t>::max()](const uint32_t curr) mutable{
            return std::exchange(prev, curr) < curr;
        });
        return std::count_if(std::begin(input), std::end(input), pred);
    }

    uint32_t count_inc_loop(const std::vector<uint32_t>& input) {
        uint32_t total{};
        uint32_t prev = std::numeric_limits<uint32_t>::max();
        for(const auto i : input) {
            if (i > prev) {
                ++total;
            }
            prev = i;
        }
        return total;
    }
}

TEST(seq, loop) {
    const auto v = std::vector{1U, 2U, 3U, 4U, 5U};
    EXPECT_EQ(count_inc_loop(v), 4) << "on input " << "1 2 3 4 5";
}

TEST(seq, non_loop) {
    const auto v = std::vector{1U, 2U, 3U, 4U, 5U};
    EXPECT_EQ(count_inc(v), 4) << "on input " << "1 2 3 4 5";
}

TEST(seq, non_loop_is) {
    const std::string input = "1 2 3 4 5";
    std::istringstream is(input);
    std::vector<uint32_t> v;
    std::copy(std::istream_iterator<uint32_t>(is),
              std::istream_iterator<uint32_t>{}, std::back_inserter(v));
    EXPECT_EQ(count_inc(v), 4) << "on input " << "1 2 3 4 5";
}


TEST(seq, ranges_sol) {
    const std::string input = "1 2 3 4 5";
    std::istringstream is(input);
    EXPECT_EQ(ranges_sol(is), 4) << "on input " << input;
}

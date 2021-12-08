//
// Created by kobic on 12/7/21.
//

#include <vector>
#include <numeric>

#include "range/v3/all.hpp"
#include "fmt/format.h"
#include "gtest/gtest.h"

/**
 * we are given an additional task. Instead of comparing every measurement,
 * we first need to compute a sliding window sum over three elements.
 * So, for example, in the case of 1 2 3 4 5 we will be comparing 1+2+3 < 2+3+4 and 2+3+4 < 3+4+5.
 */

namespace {
    uint32_t count_increasing_sliding_window(const std::vector<uint32_t>& input) {
        if (input.size() < 4) {
            return 0;
        }
        auto slide_iter = input | ranges::views::sliding(3) | ranges::views::transform([](auto&& r) {
            return ranges::accumulate(r, 0);
        });
        uint32_t total{};
        auto prev_iter{slide_iter | ranges::views::take(1)};
        uint32_t prev = *std::begin(prev_iter);
        for (const auto curr : slide_iter | ranges::views::drop(1)) {
            if (std::exchange(prev, curr) < curr) {
                ++total;
            }
        }
        return total;
    }

    uint32_t count_increasing_windows_lazy(const std::vector<uint32_t>& input) {
        if (input.size() < 4) {
            return 0;
        }
        uint32_t total{};
        uint32_t back_sum = input[0] + input[1] + input[2];
        for (uint32_t idx{3}, back_idx{0}; idx < input.size(); ++idx, ++back_idx) {
            uint32_t front_sum = back_sum + input[idx] - input[idx-3];
            if (back_sum < front_sum) {
                ++total;
            }
            back_sum = front_sum;
        }
        return total;
    }

    uint32_t count_increasing_windows_partial_loop(const std::vector<uint32_t>& input) {
        if(input.size() < 4) {
            return 0;
        }
        // partial sum
        std::vector<uint32_t> result_v(input.size());
        for (uint32_t idx{}, sum{}; idx < input.size(); ++idx) {
            sum += input[idx];
            result_v[idx] = sum;
        }
        uint32_t total{};
        for(uint32_t front{3}, back{2}; front < result_v.size(); ++front, ++back) {
            uint32_t back_diff{}, front_diff{};
            if (front > 2) {
                front_diff = result_v[front-3];
            }
            if (back > 2) {
                back_diff = result_v[back-3];
            }
            const uint32_t x = result_v[back] - back_diff;
            const uint32_t y = result_v[front] - front_diff;
            if (x < y) {
                ++total;
            }
        }
        return total;
    }

    uint32_t count_increasing_windows_partial_sum(const std::vector<uint32_t>& input) {
        if(input.size() < 4) {
            return 0;
        }
        // partial sum
        std::vector<uint32_t> result_v;
        std::partial_sum(std::begin(input), std::end(input), std::back_inserter(result_v));
        // this part is the same as partial_loop
        uint32_t total{};
        for(uint32_t front{3}, back{2}; front < result_v.size(); ++front, ++back) {
            uint32_t back_diff{}, front_diff{};
            if (front > 2) {
                front_diff = result_v[front-3];
            }
            if (back > 2) {
                back_diff = result_v[back-3];
            }
            const uint32_t x = result_v[back] - back_diff;
            const uint32_t y = result_v[front] - front_diff;
            if (x < y) {
                ++total;
            }
        }
        return total;
    }
}

TEST(SlidingTest, Lazy) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"",                                        0},     // not enough elements for a window
            {"0 1",                                     0},     // not enough elements for a window
            {"0 1 0",                                   0},     // a single window
            {"0 0 0 0 0 0",                             0},     // monotonic, no increase
            {"0 1 2 3",                                 1},     // two windows, increasing
            {"3 2 1 0",                                 0},     // two windows, decreasing
            {"0 1 0 1 0 1",                             2},     // 1, 2, 1, 2 - two increasing
            {"199 200 208 210 200 207 240 269 260 263", 5}, // from aoc
    };

    for (auto &i: inputs) {
        std::stringstream s(i.first);
        std::vector<uint32_t> v;
        std::copy(std::istream_iterator<uint32_t>(s),
                  std::istream_iterator<uint32_t>{},
                  std::back_inserter(v));
        EXPECT_EQ(count_increasing_windows_lazy(v), i.second) << " on input " << i.first;
    }
}

TEST(SlidingTest, loop) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"",                                        0},     // not enough elements for a window
            {"0 1",                                     0},     // not enough elements for a window
            {"0 1 0",                                   0},     // a single window
            {"0 0 0 0 0 0",                             0},     // monotonic, no increase
            {"0 1 2 3",                                 1},     // two windows, increasing
            {"3 2 1 0",                                 0},     // two windows, decreasing
            {"0 1 0 1 0 1",                             2},     // 1, 2, 1, 2 - two increasing
            {"199 200 208 210 200 207 240 269 260 263", 5}, // from aoc
    };

    for (auto &i: inputs) {
        std::stringstream s(i.first);
        std::vector<uint32_t> v;
        std::copy(std::istream_iterator<uint32_t>(s),
                std::istream_iterator<uint32_t>{},
                std::back_inserter(v));
        EXPECT_EQ(count_increasing_windows_partial_loop(v), i.second) << " on input " << i.first;
    }
}

TEST(SlidingTest, partial_sum) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"",                                        0},     // not enough elements for a window
            {"0 1",                                     0},     // not enough elements for a window
            {"0 1 0",                                   0},     // a single window
            {"0 0 0 0 0 0",                             0},     // monotonic, no increase
            {"0 1 2 3",                                 1},     // two windows, increasing
            {"3 2 1 0",                                 0},     // two windows, decreasing
            {"0 1 0 1 0 1",                             2},     // 1, 2, 1, 2 - two increasing
            {"199 200 208 210 200 207 240 269 260 263", 5}, // from aoc
    };

    for (auto &i: inputs) {
        std::stringstream s(i.first);
        std::vector<uint32_t> v;
        std::copy(std::istream_iterator<uint32_t>(s),
                  std::istream_iterator<uint32_t>{},
                  std::back_inserter(v));
        EXPECT_EQ(count_increasing_windows_partial_sum(v), i.second) << " on input " << i.first;
    }
}

TEST(SlidingTest, ranges_sliding) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"",                                        0},     // not enough elements for a window
            {"0 1",                                     0},     // not enough elements for a window
            {"0 1 0",                                   0},     // a single window
            {"0 0 0 0 0 0",                             0},     // monotonic, no increase
            {"0 1 2 3",                                 1},     // two windows, increasing
            {"3 2 1 0",                                 0},     // two windows, decreasing
            {"0 1 0 1 0 1",                             2},     // 1, 2, 1, 2 - two increasing
            {"199 200 208 210 200 207 240 269 260 263", 5}, // from aoc
    };

    for (auto &i: inputs) {
        std::stringstream s(i.first);
        std::vector<uint32_t> v;
        std::copy(std::istream_iterator<uint32_t>(s),
                  std::istream_iterator<uint32_t>{},
                  std::back_inserter(v));
        EXPECT_EQ(count_increasing_sliding_window(v), i.second) << " on input " << i.first;
    }
}

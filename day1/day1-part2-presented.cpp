//
// Created by kobic on 12/7/21.
//

#include <numeric>
#include "range/v3/all.hpp"

#include "gtest/gtest.h"

/**
 * we are given an additional task. Instead of comparing every measurement,
 * we first need to compute a sliding window sum over three elements.
 * So, for example, in the case of 1 2 3 4 5 we will be comparing 1+2+3 < 2+3+4 and 2+3+4 < 3+4+5.
 */

// walletfox ranges

namespace {

    uint32_t ranges_sol(const std::vector<uint32_t>& input) {
        if (input.size() < 4) {
            return 0;
        }

        // 1 2 3 4 5 6
        // [ [1, 2, 3] [ 2 3 4 ] [ 3 4 5 ] .. ]
        // [ 6, 9, ... ]
        auto slide_iter = input | ranges::views::sliding(3) | ranges::views::transform(
                [](auto&& r){
                    return ranges::accumulate(r, 0);
                }
                );

        uint32_t total{};

        auto prev_iter(slide_iter | ranges::views::take(1));
        uint32_t prev = *std::begin(prev_iter);
        for (const auto curr : slide_iter | ranges::views::drop(1)) {
            if (std::exchange(prev, curr) < curr) {
                ++total;
            }
        }
        return total;
    }


    uint32_t count_lazy(const std::vector<uint32_t> &input) {
        if (input.size() < 4) {
            return 0;
        }
        uint32_t total{};

        uint32_t back_sum = input[0] + input[1] + input[2];
        for(uint32_t idx{3}, back_idx{0}; idx < input.size(); ++idx, ++back_idx) {
            uint32_t front_sum = back_sum + input[idx] - input[idx-3];
            if(std::exchange(back_sum, front_sum) < front_sum) {
                ++total;
            }
//            if (back_sum < front_sum) {
//                ++total;
//            }
//            back_sum = front_sum;
        }

        return total;
    }


    uint32_t sliding_count(const std::vector<uint32_t>& input) {
        if (input.size() < 4) {
            return 0;
        }

        // partial sum
//        std::vector<uint32_t> result_v(input.size()); // cap at least size, size != 0
//        for(uint32_t idx{}, sum{}; idx < input.size(); ++idx) {
//            sum += input[idx];
//            result_v[idx] = sum;
//        }

        std::vector<uint32_t> result_v; // cap at least size, size != 0
        std::partial_sum(std::begin(input), std::end(input), std::back_inserter(result_v));

        // 0 1 2 3
        // 0 1 3 6
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


TEST(sliding, loop) {
    const auto input = std::vector{0U, 1U, 2U, 3U}; // 1
    EXPECT_EQ(sliding_count(input), 1) << " on input 0 1 2 3\n";
}

TEST(sliding, ranges_sol) {
    const auto input = std::vector{0U, 1U, 2U, 3U}; // 1
    EXPECT_EQ(ranges_sol(input), 1) << " on input 0 1 2 3\n";
}

TEST(sliding, count_lazy_sol) {
    const auto input = std::vector{0U, 1U, 2U, 3U}; // 1
    EXPECT_EQ(count_lazy(input), 1) << " on input 0 1 2 3\n";
}

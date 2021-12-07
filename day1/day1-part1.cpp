#include <sstream>
#include <vector>

#include "range/v3/all.hpp"
#include "NamedType/named_type.hpp"
#include "gtest/gtest.h"

namespace {
    /**
     * We are given a sequence of numbers (positive integers).
     * Our goal is to count the number of cases when a number in the series is higher than the preceding one.
     * So for example 0 0 0 0 0 the correct response would be 0 for 1 2 3 4 5 it would be 4 .
     */
    uint32_t count_increasing(std::istream &input) {
        return ranges::count_if(
                ranges::istream_view<uint32_t>(input),
                [prev = std::numeric_limits<uint32_t>::max()](const uint32_t curr) mutable
                { return std::exchange(prev, curr) < curr; });
    }

    uint32_t count_increasing(const std::vector<uint32_t>& input) {
        return std::count_if(std::begin(input), std::end(input),
                [prev = std::numeric_limits<uint32_t>::max()](const uint32_t curr) mutable
                { return std::exchange(prev, curr) < curr; });
    }

    uint32_t count_increasing_loop(const std::vector<uint32_t>& input) {
        uint32_t total{};
        uint32_t prev = std::numeric_limits<uint32_t>::max();
        for(const auto i : input) {
            if(i > prev) {
                ++total;
            }
            prev = i;
        }
        return total;
    }
}

TEST(TrivialTest, Simple) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"", 0},                // empty input 0 increasing
            {"0", 0},               // single element 0 increasing
            {"0 1", 1},             // two elements, with increase
            {"0 1 0 1 0 1 0 1", 4}, // alternating, 4 repetitions
            {"0 0 0 0 0 0", 0},     // monotonic, no increase
            {"0 1 2 3", 3},         // increasing sequence - 3 increases
            {"3 2 1 0", 0},         // decreasing sequence - 0 increases
    };

    for (auto &i : inputs) {
        std::stringstream s(i.first);
        EXPECT_EQ(count_increasing(s), i.second) << " on input " << i.first;
    }
}

TEST(TrivialTest, non_ranges) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"", 0},                // empty input 0 increasing
            {"0", 0},               // single element 0 increasing
            {"0 1", 1},             // two elements, with increase
            {"0 1 0 1 0 1 0 1", 4}, // alternating, 4 repetitions
            {"0 0 0 0 0 0", 0},     // monotonic, no increase
            {"0 1 2 3", 3},         // increasing sequence - 3 increases
            {"3 2 1 0", 0},         // decreasing sequence - 0 increases
    };

    for (auto &[i, val] : inputs) {
        std::vector<uint32_t> v;
        std::istringstream s(i);
        std::copy(std::istream_iterator<uint32_t>(s), std::istream_iterator<uint32_t>{},
                       std::back_inserter(v));
        EXPECT_EQ(count_increasing(v), val) << " on input " << i;
    }
}

TEST(TrivialTest, loop) {
    std::vector<std::pair<std::string, uint32_t>> inputs = {
            {"", 0},                // empty input 0 increasing
            {"0", 0},               // single element 0 increasing
            {"0 1", 1},             // two elements, with increase
            {"0 1 0 1 0 1 0 1", 4}, // alternating, 4 repetitions
            {"0 0 0 0 0 0", 0},     // monotonic, no increase
            {"0 1 2 3", 3},         // increasing sequence - 3 increases
            {"3 2 1 0", 0},         // decreasing sequence - 0 increases
    };

    for (auto &[i, val] : inputs) {
        std::vector<uint32_t> v;
        std::istringstream s(i);
        std::copy(std::istream_iterator<uint32_t>(s), std::istream_iterator<uint32_t>{},
                  std::back_inserter(v));
        EXPECT_EQ(count_increasing_loop(v), val) << " on input " << i;
    }
}

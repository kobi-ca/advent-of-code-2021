//
// Created by kobi on 12/10/21.
//

/**
 * https://adventofcode.com/2021/day/2
 * We are given instructions in text form. The format is line-based with a
 * single command on each line, with each command formed from a direction
 * and a numerical value.
 * forward 10
 * down 2
 * up 1
 */

#include <regex>
#include <fmt/format.h>

#include "gtest/gtest.h"

namespace {

    struct Position {
        int32_t horizontal{};
        int32_t depth{};
        // big hammer but works for == (we dont need the rest like "<" etc)
        friend auto operator<=>(const Position &, const Position &) = default;

        // op == with int32_t can be handy here

        Position& operator+=(const Position& pos) {
            horizontal += pos.horizontal;
            depth += pos.depth;
            return *this;
        }
    };

    Position navigate_non_regex(const std::string& directions) {
        Position pos;
        if (const auto loc = directions.find_first_of(' '); loc != std::string::npos) {
            std::string_view s();
            int32_t value {std::stoi(directions.c_str() + loc + 1)};
            if (directions.starts_with("forward")) {
                pos.horizontal += value;
            }
            if (directions.starts_with("up")) {
                pos.depth -= value;
            }
            if (directions.starts_with("down")) {
                pos.depth += value;
            }
        }
        return pos;
    }

    Position navigate(const std::string& directions) {
        const std::regex re("([a-z]+) ([0-9]+)");
        std::smatch bm;
        Position pos;
        if (std::regex_match(directions, bm, re)) {
            if (bm.size() == 3) {
                auto command = bm[1].str();
                auto val_s = bm[2].str();
                int32_t value {};
                std::istringstream is(val_s);
                is >> value;
                if (command == "forward") {
                    pos.horizontal += value;
                }
                if (command == "up") {
                    pos.depth -= value;
                }
                if (command == "down") {
                    pos.depth += value;
                }
            }
        }
        return pos;
    }
}

TEST(NavigationTest, Single) {
    std::vector<std::pair<std::string, Position>> inputs = {
            {"forward 10", {.horizontal = 10, .depth = 0}},
            {"down 5", {.horizontal = 0, .depth = 5}},
            {"forward 8", {.horizontal = 8, .depth = 0}},
            {"up 3", {.horizontal = 0, .depth = -3}},
            {"down 8", {.horizontal = 0, .depth = 8}},
            {"forward 2", {.horizontal = 2, .depth = 0}},
    };

    for (auto &i : inputs) {
        EXPECT_EQ(navigate(i.first), i.second) << "with input " << i.first;
    }
}

TEST(NavigationTest, Complete) {
    const std::string input = R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";
    std::string line;
    std::istringstream is(input);
    Position result;
    while(std::getline(is, line)) {
        result += navigate(line);
    }
    // or operator == with int32_t
    EXPECT_EQ(result.depth * result.horizontal, 150) << "with input:\n" << input;
}

TEST(NavigationTest, CompleteNonRegex) {
    const std::string input = R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";
    std::string line;
    std::istringstream is(input);
    Position result;
    while(std::getline(is, line)) {
        result += navigate_non_regex(line);
    }
    // or operator == with int32_t
    EXPECT_EQ(result.depth * result.horizontal, 150) << "with input:\n" << input;
}

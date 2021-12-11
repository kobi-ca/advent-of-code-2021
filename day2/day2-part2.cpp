//
// Created by kobi on 12/10/21.
//

/**
 * The second part of the problem introduces an interesting twist.
 * Instead of up and down representing a direct movement, they change the direction,
 * with forward then applying this direction.
 */
#include <regex>

#include "gtest/gtest.h"

namespace {

    struct Position {
        int32_t horizontal{};
        int32_t depth{};
        int32_t direction{};

        // big hammer but works for == (we dont need the rest like "<" etc)
        friend auto operator<=>(const Position &, const Position &) = default;

        void forward(const int32_t val) {
            horizontal += val;
            depth += direction * val;
        }

        void up(const int32_t val) {
            direction -= val;
        }

        void down(const int32_t val) {
            direction += val;
        }
    };

    Position& navigate(Position& position, const std::string& directions) {
        const std::regex re("([a-z]+) ([0-9]+)");
        std::smatch bm;
        if (std::regex_match(directions, bm, re)) {
            if (bm.size() == 3) {
                auto command = bm[1].str();
                auto val_s = bm[2].str();
                int32_t value {};
                std::istringstream is(val_s);
                is >> value;
                if (command == "forward") {
                    position.forward(value);
                }
                if (command == "up") {
                    position.up(value);
                }
                if (command == "down") {
                    position.down(value);
                }
            }
        }
        return position;
    }
}

TEST(NavigationTest, Single) {
    std::vector<std::pair<std::string, Position>> inputs = {
            {"forward 10", {.horizontal = 10, .depth = 0, .direction = 0}},
            {"down 10", {.horizontal = 0, .depth = 0, .direction = 10}},
            {"up 10", {.horizontal = 0, .depth = 0, .direction = -10}},
    };

    for (auto &i : inputs) {
        Position pos;
        EXPECT_EQ(navigate(pos, i.first), i.second) << "with input " << i.first;
    }
}

TEST(NavigationTest, Complete) {
    std::vector<std::pair<std::string, Position>> inputs = {
            {R"(forward 10
down 5
up 2
forward 2)", {.horizontal = 12, .depth = 6, .direction = 3}},
            {R"(down 20
down 20
up 40
forward 0)", {.horizontal = 0, .depth = 0, .direction = 0}},
            {R"(down 20
down 20
forward 2)", {.horizontal = 2, .depth = 80, .direction = 40}},
    };
    for (auto &[str, position] : inputs) {
        Position pos;
        EXPECT_EQ(navigate(pos, str), position) << "with input " << str;
    }
}

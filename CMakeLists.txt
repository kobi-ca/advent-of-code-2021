cmake_minimum_required(VERSION 3.21)
project(advent_of_code)

set(CMAKE_CXX_STANDARD 20)

find_package(gtest REQUIRED)

add_executable(day1-part1 day1/day1-part1.cpp)
target_link_libraries(day1-part1 PRIVATE gtest::gtest)

cmake_minimum_required(VERSION 3.21)
project(advent_of_code)

set(CMAKE_CXX_STANDARD 20)

find_package(gtest REQUIRED)
find_package(namedtype REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(day1-part1 day1/day1-part1.cpp)
target_link_libraries(day1-part1 PRIVATE namedtype::namedtype range-v3::range-v3 gtest::gtest)

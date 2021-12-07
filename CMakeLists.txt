cmake_minimum_required(VERSION 3.21)
project(advent_of_code)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(namedtype REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(day1-part1 day1/day1-part1.cpp)
target_link_libraries(day1-part1 PRIVATE namedtype::namedtype range-v3::range-v3 gtest_main)

add_executable(day1-part1-presented day1/day1-part1-presented.cpp)
target_link_libraries(day1-part1-presented PRIVATE namedtype::namedtype range-v3::range-v3 gtest_main)

add_executable(day1-part2 day1/day1-part2.cpp)
target_link_libraries(day1-part2 PRIVATE namedtype::namedtype range-v3::range-v3 gtest_main)

add_executable(day1-part2-presented day1/day1-part2-presented.cpp)
target_link_libraries(day1-part2-presented PRIVATE namedtype::namedtype range-v3::range-v3 gtest_main)

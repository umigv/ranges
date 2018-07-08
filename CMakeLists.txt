cmake_minimum_required(VERSION 3.1)

project(ranges)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
find_package(GTest REQUIRED)

include_directories(include/ ${GTEST_INCLUDE_DIRS})

install(DIRECTORY include/ DESTINATION include/umigv/ranges)

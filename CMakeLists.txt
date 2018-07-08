cmake_minimum_required(VERSION 3.1)

project(ranges)

option(UMIGV_RANGES_BUILD_TESTS "Build tests for ranges." OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include/)

if (UMIGV_RANGES_BUILD_TESTS)
    enable_testing()

    find_package(GTest REQUIRED)

    include_directories(include/ ${GTEST_INCLUDE_DIRS})

    add_executable(test_iterator_range test/iterator_range.cpp)
    target_link_libraries(test_iterator_range ${GTEST_BOTH_LIBRARIES})

    add_test(TestIteratorRange test_iterator_range)
endif()

install(DIRECTORY include/ DESTINATION include/umigv/ranges)

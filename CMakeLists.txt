cmake_minimum_required(VERSION 3.1)

project(ranges)

option(UMIGV_RANGES_BUILD_TESTS "Build tests for ranges." OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include/)

if (UMIGV_RANGES_BUILD_TESTS)
    add_subdirectory(googletest)

    enable_testing()

    include_directories(include/ ${gtest_SOURCE_DIR}
                        ${gtest_SOURCE_DIR}/include/)

    add_executable(test_iterator_range test/iterator_range.cpp)
    target_link_libraries(test_iterator_range gtest gtest_main)

    add_executable(test_mapped_range test/mapped_range.cpp)
    target_link_libraries(test_mapped_range gtest gtest_main)

    add_test(TestIteratorRange test_iterator_range)
    add_test(TestMappedRange test_mapped_range)
endif()

install(DIRECTORY include/ DESTINATION include/umigv/ranges)

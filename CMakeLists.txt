cmake_minimum_required(VERSION 3.1)

project(ranges)

option(UMIGV_RANGES_BUILD_TESTS "Build tests for ranges." OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include/)

if (UMIGV_RANGES_BUILD_TESTS)
    set(__CFLAGS "${CMAKE_C_FLAGS}")
    set(__CXXFLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "")
    set(CMAKE_CXX_FLAGS "")
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
    set(CMAKE_C_FLAGS "${__CFLAGS}")
    set(CMAKE_CXX_FLAGS "${__CXXFLAGS}")
    unset(__CFLAGS)
    unset(__CXXFLAGS)

    enable_testing()

    include_directories(include/ ${gtest_SOURCE_DIR}/include/)

    add_executable(test_iterator_range test/iterator_range.cpp)
    target_link_libraries(test_iterator_range gtest gtest_main)

    add_executable(test_mapped_range test/mapped_range.cpp)
    target_link_libraries(test_mapped_range gtest gtest_main)

    add_executable(test_filtered_range test/filtered_range.cpp)
    target_link_libraries(test_filtered_range gtest gtest_main)

    add_executable(test_counting_range test/counting_range.cpp)
    target_link_libraries(test_counting_range gtest gtest_main)

    add_executable(test_collect test/collect.cpp)
    target_link_libraries(test_collect gtest gtest_main)

    add_test(TestIteratorRange test_iterator_range)
    add_test(TestMappedRange test_mapped_range)
    add_test(TestFilteredRange test_filtered_range)
    add_test(TestCountingRange test_counting_range)
    add_test(TestCollect test_collect)
endif()

install(DIRECTORY include/ DESTINATION include/umigv/ranges)

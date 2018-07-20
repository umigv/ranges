cmake_minimum_required(VERSION 3.1)

project(ranges)

option(UMIGV_RANGES_BUILD_TESTS "Build tests for ranges." OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(type_safe)
set(type_safe_INCLUDE_DIRS type_safe/include/ type_safe/external/debug_assert)

include_directories(include/ ${type_safe_INCLUDE_DIRS})

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

    include_directories(${gtest_INCLUDE_DIRS})

    add_executable(test_range_adapter test/range_adapter.cpp)
    target_link_libraries(test_range_adapter gtest gtest_main)

    add_executable(test_mapped_range test/mapped_range.cpp)
    target_link_libraries(test_mapped_range gtest gtest_main)

    add_executable(test_filtered_range test/filtered_range.cpp)
    target_link_libraries(test_filtered_range gtest gtest_main)

    add_executable(test_counting_range test/counting_range.cpp)
    target_link_libraries(test_counting_range gtest gtest_main)

    add_executable(test_collect test/collect.cpp)
    target_link_libraries(test_collect gtest gtest_main)

    add_executable(test_const_iterator test/const_iterator.cpp)
    target_link_libraries(test_const_iterator gtest gtest_main)

    add_executable(test_apply test/apply.cpp)
    target_link_libraries(test_apply gtest gtest_main)

    add_executable(test_enumerated_range test/enumerated_range.cpp)
    target_link_libraries(test_enumerated_range gtest gtest_main)

    add_executable(test_zipped_range test/zipped_range.cpp)
    target_link_libraries(test_zipped_range gtest gtest_main)

    add_test(TestRangeAdapter test_range_adapter)
    add_test(TestMappedRange test_mapped_range)
    add_test(TestFilteredRange test_filtered_range)
    add_test(TestCountingRange test_counting_range)
    add_test(TestCollect test_collect)
    add_test(TestConstIterator test_const_iterator)
    add_test(TestApply test_apply)
    add_test(TestEnumeratedRange test_enumerated_range)
    add_test(TestZippedRange test_zipped_range)
endif()

install(DIRECTORY include/ DESTINATION include/umigv/ranges)

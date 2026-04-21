cmake_minimum_required(VERSION 3.30)

project(purple-arithmetics VERSION 0.3.0 LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

include(FetchContent)
FetchContent_Declare(
    hegel
    GIT_REPOSITORY https://github.com/hegeldev/hegel-cpp.git
    GIT_TAG v0.3.5
)
FetchContent_MakeAvailable(hegel)

# main

find_package(benchmark REQUIRED)
find_package(fmt REQUIRED)
find_package(gmp REQUIRED)
find_package(GTest REQUIRED)

add_library(purple-arithmetic)

FILE(GLOB main_cxx_sources src/main/cxx/*.cxx)
FILE(GLOB main_ixx_sources src/main/ixx/*.ixx)

target_sources(purple-arithmetic
    PUBLIC FILE_SET CXX_MODULES FILES
    ${main_ixx_sources}
    PRIVATE
    ${main_cxx_sources}
)

# utility

add_library(purple-arithmetic-utility)

FILE(GLOB_RECURSE utility_cxx_sources src/utility/cxx/*.cxx)
FILE(GLOB utility_ixx_sources src/utility/ixx/*.ixx)

target_sources(purple-arithmetic-utility
        PUBLIC FILE_SET CXX_MODULES FILES
        ${utility_ixx_sources}
        PRIVATE
        ${utility_cxx_sources}
)

target_link_libraries(purple-arithmetic-utility
        PRIVATE
        fmt::fmt
        gmp::gmp
        purple-arithmetic
)

# test

add_executable(purple-arithmetic-test)

FILE(GLOB_RECURSE test_cxx_sources src/test/cxx/*.cxx)
FILE(GLOB test_ixx_sources src/test/ixx/*.ixx)

target_sources(purple-arithmetic-test
    PUBLIC FILE_SET CXX_MODULES FILES
    ${test_ixx_sources}
    PRIVATE
    ${test_cxx_sources}
)

target_link_libraries(purple-arithmetic-test
    PRIVATE
    fmt::fmt
    hegel
    gmp::gmp
    GTest::gtest_main
    purple-arithmetic
    purple-arithmetic-utility
)

add_test(NAME purple-arithmetic-test COMMAND purple-arithmetic-test)

# benchmark

add_executable(purple-arithmetic-benchmark)

FILE(GLOB benchmark_cxx_sources src/benchmark/cxx/*.cxx)

target_sources(purple-arithmetic-benchmark
        PRIVATE
        ${benchmark_cxx_sources}
)

target_link_libraries(purple-arithmetic-benchmark
        PRIVATE
        benchmark::benchmark
        fmt::fmt
        gmp::gmp
        purple-arithmetic
)

# fuzz

add_executable(purple-arithmetic-fuzz)

FILE(GLOB fuzz_cxx_sources src/fuzz/cxx/*.cxx)

target_sources(purple-arithmetic-fuzz
    PRIVATE
    ${fuzz_cxx_sources}
)

target_link_libraries(purple-arithmetic-fuzz
    PRIVATE
    fmt::fmt
    gmp::gmp
    purple-arithmetic
    purple-arithmetic-utility
)

cmake_minimum_required(VERSION 3.30)

project(purple-arithmetic VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

# main

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
    gmp::gmp
    GTest::gtest_main
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
    purple-arithmetic
)

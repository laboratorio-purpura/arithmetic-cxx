cmake_minimum_required(VERSION 3.30)

project(purple-crypto VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 23)

# main

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)

add_library(purple-crypto-api)

FILE(GLOB main_cxx_sources src/main/cxx/*.cxx)
FILE(GLOB main_ixx_sources src/main/ixx/*.ixx)

target_sources(purple-crypto-api
    PUBLIC FILE_SET CXX_MODULES FILES
    ${main_ixx_sources}
    PRIVATE
    ${main_cxx_sources}
)

# test

add_executable(purple-crypto-test)

FILE(GLOB test_cxx_sources src/test/cxx/*.cxx)
FILE(GLOB test_ixx_sources src/test/ixx/*.ixx)

target_sources(purple-crypto-test
    PUBLIC FILE_SET CXX_MODULES FILES
    ${test_ixx_sources}
    PRIVATE
    ${test_cxx_sources}
)

target_link_libraries(purple-crypto-test
    PRIVATE
    fmt::fmt
    GTest::gtest_main
    purple-crypto-api
)

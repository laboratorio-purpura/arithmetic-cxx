set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

find_package(fmt REQUIRED)
find_package(gmp REQUIRED)
find_package(GTest REQUIRED)

add_executable(purple-arithmetics-test)

FILE(GLOB_RECURSE test_cxx_sources cxx/*.cxx)
FILE(GLOB test_ixx_sources ixx/*.ixx)

target_sources(purple-arithmetics-test
    PUBLIC FILE_SET CXX_MODULES FILES
    ${test_ixx_sources}
    PRIVATE
    ${test_cxx_sources}
)

target_link_libraries(purple-arithmetics-test
    PRIVATE
    fmt::fmt
    hegel
    gmp::gmp
    GTest::gtest_main
    purple-arithmetics
    purple-arithmetics-utility
)

add_test(NAME purple-arithmetics-test COMMAND purple-arithmetics-test)

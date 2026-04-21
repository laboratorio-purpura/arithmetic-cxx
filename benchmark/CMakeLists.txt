set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

find_package(benchmark REQUIRED)
find_package(fmt REQUIRED)
find_package(gmp REQUIRED)

add_executable(purple-arithmetics-benchmark)

FILE(GLOB cxx_sources cxx/*.cxx)

target_sources(purple-arithmetics-benchmark
    PRIVATE
    ${cxx_sources}
)

target_link_libraries(purple-arithmetics-benchmark
    PRIVATE
    benchmark::benchmark
    fmt::fmt
    gmp::gmp
    purple-arithmetics
)

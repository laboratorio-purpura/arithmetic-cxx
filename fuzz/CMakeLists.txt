set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

find_package(fmt REQUIRED)
find_package(gmp REQUIRED)

add_executable(purple-arithmetics-fuzz)

FILE(GLOB cxx_sources cxx/*.cxx)

target_sources(purple-arithmetics-fuzz
    PRIVATE
    ${cxx_sources}
)

target_link_libraries(purple-arithmetics-fuzz
    PRIVATE
    fmt::fmt
    gmp::gmp
    purple-arithmetics
    purple-arithmetics-utility
)

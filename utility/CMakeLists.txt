set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

find_package(fmt REQUIRED)
find_package(gmp REQUIRED)

add_library(purple-arithmetics-utility)

FILE(GLOB utility_cxx_sources cxx/*.cxx)
FILE(GLOB utility_ixx_sources ixx/*.ixx)

target_sources(purple-arithmetics-utility
    PUBLIC FILE_SET CXX_MODULES FILES
    ${utility_ixx_sources}
    PRIVATE
    ${utility_cxx_sources}
)

target_link_libraries(purple-arithmetics-utility
    PRIVATE
    fmt::fmt
    gmp::gmp
    purple-arithmetics
)

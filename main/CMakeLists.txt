set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

add_library(purple-arithmetics)

FILE(GLOB cxx_sources cxx/*.cxx)
FILE(GLOB ixx_sources ixx/*.ixx)

target_sources(purple-arithmetics
    PUBLIC FILE_SET CXX_MODULES FILES
    ${ixx_sources}
    PRIVATE
    ${cxx_sources}
)

cmake_minimum_required(VERSION 3.30)

project(purple-arithmetics VERSION 0.4.0 LANGUAGES C CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD 26)

include(FetchContent)
FetchContent_Declare(
    hegel
    GIT_REPOSITORY https://github.com/hegeldev/hegel-cpp.git
    GIT_TAG v0.3.8
)
FetchContent_MakeAvailable(hegel)

add_subdirectory(benchmark)
add_subdirectory(fuzz)
add_subdirectory(main)
add_subdirectory(test)
add_subdirectory(utility)

cmake_minimum_required(VERSION 3.15)
project(HSharpVE CXX)

set(EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

find_package(GTest REQUIRED CONFIG)
find_package(eternal REQUIRED CONFIG)

set(CMAKE_CXX_FLAGS "-fsanitize=address,leak -O0 -ggdb3")

add_subdirectory(src)
add_subdirectory(tests)

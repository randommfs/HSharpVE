cmake_minimum_required(VERSION 3.15)
project(HSharpVE CXX)

find_package(GTest REQUIRED CONFIG)
find_package(eternal REQUIRED CONFIG)

add_subdirectory(src)
add_subdirectory(tests)

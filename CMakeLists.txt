cmake_minimum_required(VERSION 3.24)

project(
    SYMBOLIC
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "Symbolic math library in C++, for fun and probably not profit"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
add_subdirectory(test)
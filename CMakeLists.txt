project(
    SYMBOLIC
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "Symbolic math library in C++, for fun and probably not profit"
)

set(CXX_STANDARD_REQUIRED C++20)

add_subdirectory(src)
add_subdirectory(test)
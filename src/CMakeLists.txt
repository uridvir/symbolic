add_library(symbolic
    ${CMAKE_CURRENT_SOURCE_DIR}/symbolic.cpp
)

target_include_directories(symbolic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
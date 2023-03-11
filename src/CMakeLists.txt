add_library(symbolic OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/symbolic.cpp
)

target_include_directories(symbolic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
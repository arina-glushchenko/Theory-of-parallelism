cmake_minimum_required(VERSION 3.10)

project(DoubleOrFloatExample)

add_executable(main main.cpp)

if(BUILD_TYPE STREQUAL "float")
    add_compile_definitions(FLOAT)
elseif(BUILD_TYPE STREQUAL "double")
    add_compile_definitions(DOUBLE)
endif()
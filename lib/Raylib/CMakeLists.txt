cmake_minimum_required(VERSION 3.10)
project(raylib_minimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp)

add_library(raylib_minimal SHARED ${SOURCES})

target_include_directories(raylib_minimal PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/include
)

set_target_properties(raylib_minimal PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

target_link_libraries(raylib_minimal PRIVATE raylib)

target_compile_options(raylib_minimal PRIVATE -fno-gnu-unique)

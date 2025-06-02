cmake_minimum_required(VERSION 3.10)
project(raylibRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp)

add_library(raylibRenderer SHARED ${SOURCES})

target_include_directories(raylibRenderer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/include
)

set_target_properties(raylibRenderer PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

target_link_libraries(raylibRenderer PRIVATE raylib)

target_compile_options(raylibRenderer PRIVATE -fno-gnu-unique)

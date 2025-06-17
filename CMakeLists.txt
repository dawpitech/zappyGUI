cmake_minimum_required(VERSION 3.10)
project(ZappyGUI LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/core/Core.cpp
    src/communication/CommunicationBuffer.cpp
    src/network/NetworkManager.cpp
    src/raylib/Raylib.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/Raylib/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/Raylib/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

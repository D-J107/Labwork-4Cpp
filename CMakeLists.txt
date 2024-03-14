cmake_minimum_required(VERSION 3.12)

project(
        labwork4
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 labwork4"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)

add_subdirectory(lib)
add_subdirectory(bin)
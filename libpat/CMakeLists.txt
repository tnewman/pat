cmake_minimum_required(VERSION 3.10)
project(libpat C)

set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

find_package(FFmpeg)
find_package(SDL2)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(libpat)
add_subdirectory(test)

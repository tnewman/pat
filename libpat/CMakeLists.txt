cmake_minimum_required(VERSION 3.10)
project(libpat C)

set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

find_package(PythonLibs 3.6 REQUIRED)
find_package(FFmpeg)
find_package(SDL2)

if(NOT ${Python3_FOUND})
    message(FATAL_ERROR "Python 3 not found!")
endif()

if(NOT ${FFMPEG_FOUND})
    message(FATAL_ERROR "FFMPEG not found!")
endif()

if(NOT ${SDL2_FOUND})
    message(FATAL_ERROR "SDL2 not found!")
endif()

add_subdirectory(libpat)
add_subdirectory(test)
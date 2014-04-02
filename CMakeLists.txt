project(final)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE off)

if(APPLE)
    set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/osx)
elseif(UNIX)
    set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/linux)
else()
    message(FATAL_ERROR "Platform not supported")
endif()

set(${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory("src")
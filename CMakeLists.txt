project(velonaut)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE off)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT UNIX)
    message(FATAL_ERROR "Platform not supported")
endif()

if(NOT APPLE)
    set(CMAKE_CXX_COMPILER "g++-4.8")
endif()

set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_NAME})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(cotire)

add_subdirectory("src/engine")

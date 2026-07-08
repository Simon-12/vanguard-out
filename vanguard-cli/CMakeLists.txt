cmake_minimum_required(VERSION 3.20)

project(vanguard-cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CLI11
if(DEFINED CLI11_INCLUDE_DIR)
    include_directories(${CLI11_INCLUDE_DIR})
    # Alternative if installed
    # find_package(CLI11 CONFIG REQUIRED)
else()
    message(WARNING "CLI11_INCLUDE_DIR is not defined!")
endif()

# mINI
if(DEFINED mINI_INCLUDE_DIR)
    include_directories(${mINI_INCLUDE_DIR})
else()
    message(WARNING "mINI_INCLUDE_DIR is not defined!")
endif()

add_executable(vanguard-cli main.cpp)

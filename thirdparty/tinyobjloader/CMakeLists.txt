set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.20)
project(tinyobjloader)

add_library(tinyobjloader INTERFACE)

target_include_directories(tinyobjloader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
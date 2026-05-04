set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.20)
project(stb_image)

add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
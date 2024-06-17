set(CMAKE_CXX_STANDARD 23)

add_library(TempLib temp.cpp temp.h)

target_include_directories(TempLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
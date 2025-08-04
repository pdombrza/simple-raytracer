set(CMAKE_CXX_STANDARD 23)
add_library(Window window.cpp window.h windinput.h windinput.cpp)

target_include_directories(Window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
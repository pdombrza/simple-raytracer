set(CMAKE_CXX_STANDARD 23)
add_library(Tobmp tobmp.cpp tobmp.h)

target_include_directories(Tobmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
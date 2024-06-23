set(CMAKE_CXX_STANDARD 23)
add_library(Color color.cpp color.h)

target_link_libraries(Color PRIVATE glm)

target_include_directories(Color PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
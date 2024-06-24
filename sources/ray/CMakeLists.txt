set(CMAKE_CXX_STANDARD 23)
add_library(Ray ray.cpp ray.h)

target_link_libraries(Ray PRIVATE glm)

target_include_directories(Ray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 23)
add_library(Ray ray.cu ray.h)

target_link_libraries(Ray PRIVATE glm Cuvec)
target_include_directories(Ray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_include_directories(Ray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Ray PUBLIC ${PROJECT_SOURCE_DIR}/sources)
set(CMAKE_CXX_STANDARD 23)
add_library(Camera camera.cpp camera.h)

target_link_libraries(Camera PRIVATE glm)

target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Camera PUBLIC ${PROJECT_SOURCE_DIR}/sources)
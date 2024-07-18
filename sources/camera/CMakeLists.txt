set(CMAKE_CXX_STANDARD 23)
add_library(Camera camera.cpp camera.h)

target_link_libraries(Camera PRIVATE glm)

target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
target_include_directories(Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hitrec")
target_include_directories(Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hittable")
target_include_directories(Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../utils")
target_include_directories(Camera PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../material")
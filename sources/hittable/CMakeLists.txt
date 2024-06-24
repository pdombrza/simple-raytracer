set(CMAKE_CXX_STANDARD 23)
add_library(Hittable hittable.cpp hittable.h)

target_link_libraries(Hittable PRIVATE glm)

target_include_directories(Hittable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Hittable PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
target_include_directories(Hittable PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hitrec")
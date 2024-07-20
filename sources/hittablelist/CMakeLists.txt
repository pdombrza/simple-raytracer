set(CMAKE_CXX_STANDARD 23)
add_library(HittableList hittablelist.cpp hittablelist.h)

target_link_libraries(HittableList PRIVATE glm)

target_include_directories(HittableList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(HittableList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
target_include_directories(HittableList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hitrec")
target_include_directories(HittableList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hittable")
target_include_directories(HittableList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../scattering_record")
target_include_directories(HittableList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../utils")
target_include_directories(HittableList PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../material")
set(CMAKE_CXX_STANDARD 23)
add_library(Material material.cpp material.h)

target_link_libraries(Material PRIVATE glm)

target_include_directories(Material PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Material PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
target_include_directories(Material PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hitrec")
target_include_directories(Material PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../utils")
target_include_directories(Material PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../scattering_record")
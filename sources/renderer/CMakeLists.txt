set(CMAKE_CXX_STANDARD 23)
add_library(Renderer renderer.cpp renderer.h)

target_link_libraries(Renderer PRIVATE glm)

add_compile_definitions(SOURCE_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hitrec")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hittable")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../hittablelist")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../camera")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../tobmp")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../color")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../utils")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../material")
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../scattering_record")
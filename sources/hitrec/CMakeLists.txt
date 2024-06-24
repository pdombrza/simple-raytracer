set(CMAKE_CXX_STANDARD 23)
add_library(HitRecord hitrec.cpp hitrec.h)

target_link_libraries(HitRecord PRIVATE glm)

target_include_directories(HitRecord PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(HitRecord PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
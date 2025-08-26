set(CMAKE_CXX_STANDARD 23)
add_library(HittableList hittablelist.cpp hittablelist.h)

target_link_libraries(HittableList PRIVATE glm)

target_include_directories(HittableList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(HittableList PUBLIC ${PROJECT_SOURCE_DIR}/sources)
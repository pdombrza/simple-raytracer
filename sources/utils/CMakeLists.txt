set(CMAKE_CXX_STANDARD 23)
add_library(Utils utils.cpp utils.h)

target_link_libraries(Utils PRIVATE glm)

target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
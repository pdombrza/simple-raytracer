set(CMAKE_CXX_STANDARD 23)
add_library(ScatteringRecord scattering_record.cpp scattering_record.h)

target_link_libraries(ScatteringRecord PRIVATE glm)

target_include_directories(ScatteringRecord PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ScatteringRecord PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ray")
target_include_directories(ScatteringRecord PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../utils")
set(CMAKE_CXX_STANDARD 23)
add_library(Cubemap cubemap.cpp cubemap.h)

target_link_libraries(Cubemap PRIVATE CUDA::cudart CUDA::cuda_driver glm stb_image)

target_include_directories(Cubemap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Cubemap PUBLIC ${PROJECT_SOURCE_DIR}/sources)
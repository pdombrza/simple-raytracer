set(CMAKE_CXX_STANDARD 23)
add_library(Cudaray curaytracer.cu cudaray.h)

target_link_libraries(Cudaray PRIVATE CUDA::cudart CUDA::cuda_driver glm)
target_include_directories(Cudaray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
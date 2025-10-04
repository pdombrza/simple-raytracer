set(CMAKE_CXX_STANDARD 23)
add_library(Cudaray curaytracer.cu cudaray.h)

target_link_libraries(Cudaray PRIVATE CUDA::cudart CUDA::cuda_driver glm Ray Hittable HittableList Utils)

target_include_directories(Cudaray PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Cudaray PUBLIC ${PROJECT_SOURCE_DIR})

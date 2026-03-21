set(CMAKE_CXX_STANDARD 23)
add_library(Mesh mesh.cu mesh.h)

target_link_libraries(Mesh PRIVATE CUDA::cudart CUDA::cuda_driver glm Hittable Material)

target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Mesh PUBLIC ${PROJECT_SOURCE_DIR}/sources)

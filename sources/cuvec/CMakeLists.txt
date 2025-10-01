set(CMAKE_CXX_STANDARD 23)
add_library(Cuvec cuvec.cu cuvec.h)

target_link_libraries(Cuvec PRIVATE CUDA::cudart CUDA::cuda_driver glm)

target_include_directories(Cuvec PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Cuvec PUBLIC ${PROJECT_SOURCE_DIR}/sources)

#include "cudaray.h"

#define checkCudaErrors(val) checkCuda( (val), #val, __FILE__, __LINE__ )

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
            file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}

__global__ void renderKernel(glm::vec3* buffer, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int index = y * width + x;
    buffer[index] = glm::vec3(0.0f, 1.0f, 0.0f);
}

std::unique_ptr<glm::vec3[]> launchRaytracer(int width, int height) {
    size_t size = width * height * sizeof(glm::vec3);

    glm::vec3* buffer = nullptr;
    checkCudaErrors(cudaMalloc(&buffer, size));
    dim3 blockSize(16, 16);
    dim3 gridSize((width + 15) / 16, (height + 15) / 16);
    renderKernel<<<gridSize, blockSize>>>(buffer, width, height);
    checkCudaErrors(cudaDeviceSynchronize());

    std::unique_ptr<glm::vec3[]> pxBuffer = std::make_unique<glm::vec3[]>(width * height);

    checkCudaErrors(cudaMemcpy(pxBuffer.get(), buffer, size, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(buffer));

    return pxBuffer;
}
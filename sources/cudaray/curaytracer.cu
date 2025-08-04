#include <device_launch_parameters.h>

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

__global__ void renderKernel(uint8_t* buffer, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int index = (y * width + x) * 4;
    buffer[index + 0] = 0;
    buffer[index + 1] = 0;
    buffer[index + 2] = 255;
    buffer[index + 3] = 255;
}

void launchRaytracer(uint8_t* output, int width, int height) {
    uint8_t* buffer;
    size_t size = width * height * 4;
    checkCudaErrors(cudaMalloc(&buffer, size));

    dim3 blockSize(16, 16);
    dim3 gridSize((width + blockSize.x - 1) / blockSize.x,
        (height + blockSize.y - 1) / blockSize.y);

    renderKernel<<<gridSize, blockSize>>>(buffer, width, height);
    checkCudaErrors(cudaDeviceSynchronize());
    checkCudaErrors(cudaMemcpy(output, buffer, size, cudaMemcpyDeviceToHost));
    checkCudaErrors(cudaFree(buffer));
}
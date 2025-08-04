#pragma once
#include <iostream>
#include <cuda_runtime.h>

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line);
__global__ void renderKernel(uint8_t* buffer, int width, int height);
void launchRaytracer(uint8_t* output, int width, int height);
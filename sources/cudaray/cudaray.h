#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "ray/ray.h"

#define checkCudaErrors(val) checkCuda( (val), #val, __FILE__, __LINE__ )

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line);
__device__ cu::vec3 color(const Ray&r);
__global__ void render(cu::vec3* fb, int x, int y, cu::vec3 bottomLeftCorner, cu::vec3 horizontal, cu::vec3 vertical, cu::vec3 origin);
void launchRenderer(cu::vec3* fb, int nx, int ny, int xBlock, int yBlock);
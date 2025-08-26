#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "cuvec.h"

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line);
__global__ void renderKernel(glm::vec3* buffer, int width, int height);
std::unique_ptr<glm::vec3[]> launchRaytracer(int width, int height);
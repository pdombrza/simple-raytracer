#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "ray/ray.h"
#include "hittable/hittable.h"
#include "hittablelist/hittablelist.h"
#include "cuvec/cuvec.h"
#include "hitrec/hitrec.h"

#define checkCudaErrors(val) checkCuda( (val), #val, __FILE__, __LINE__ )

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line);
__device__ cu::vec3 color(const Ray& ray, HittableList* world);
__global__ void render(cu::vec3* fb, int x, int y, cu::vec3 bottomLeftCorner, cu::vec3 horizontal, cu::vec3 vertical, cu::vec3 origin, HittableList* world);
void launchRenderer(cu::vec3* fb, int nx, int ny, int xBlock, int yBlock);
__global__ void createWorld(Hittable** d_List, HittableList* d_World);
__global__ void destroyWorld(Hittable** d_List, HittableList* d_World);
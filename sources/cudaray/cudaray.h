#pragma once
#include <iostream>

#include <glm/glm.hpp>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <curand_kernel.h>

#include "ray/ray.h"
#include "hittable/hittable.h"
#include "hittablelist/hittablelist.h"
#include "hitrec/hitrec.h"
#include "utils/utils.h"

#define checkCudaErrors(val) checkCuda( (val), #val, __FILE__, __LINE__ )

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line);
__device__ glm::vec3 color(const Ray& ray, HittableList* world, utils::random::RNG& rng);
__device__ glm::vec3 colorPixel(int i, int j, int nx, int ny, HittableList* world, utils::random::RNG& rng, glm::vec3 origin, glm::vec3 horizontal, glm::vec3 vertical, glm::vec3 bottomLeftCorner);
__global__ void render(glm::vec3* fb, int x, int y, glm::vec3 bottomLeftCorner, glm::vec3 horizontal, glm::vec3 vertical, glm::vec3 origin, HittableList* world, curandState* randState);
void launchRenderer(glm::vec3* fb, int nx, int ny, int xBlock, int yBlock);
__global__ void createWorld(Hittable** d_List, HittableList* d_World);
__global__ void destroyWorld(Hittable** d_List, HittableList* d_World);
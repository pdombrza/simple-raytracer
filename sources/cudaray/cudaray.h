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
#include "camera/camera.h"
#include "material/material.h"

#define checkCudaErrors(val) checkCuda( (val), #val, __FILE__, __LINE__ )

void checkCuda(cudaError_t result, char const* const func, const char* const file, int const line);
__device__ glm::vec3 color(const Ray& ray, HittableList* world, utils::random::RNG& rng);
__device__ glm::vec3 colorPixel(int i, int j, int nx, int ny, Camera* camera, HittableList* world, utils::random::RNG& rng);
__global__ void render(glm::vec3* fb, int x, int y, Camera* camera, HittableList* world, curandState* randState);
void launchRenderer(glm::vec3* fb, int nx, int ny, int xBlock, int yBlock);
__global__ void initCamera(Camera* cam, int width, int height);
__global__ void createWorld(Hittable** d_List, HittableList* d_World);
__global__ void destroyWorld(Hittable** d_List, HittableList* d_World);
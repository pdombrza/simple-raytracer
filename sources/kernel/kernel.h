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
#include "framebuffer/framebuffer.h"
#include "mesh/mesh.h"

__global__ void renderScene(Framebuffer* d_Fb, Camera* camera, HittableList* world, curandState* randState, int frameIndex, cudaTextureObject_t cubemapTexture,	 cudaSurfaceObject_t surfObj = 0);
__global__ void initCamera(Camera* cam, int width, int height);
__global__ void createWorld(Hittable** d_List, HittableList* d_World, glm::vec3* vertexArray, int* indexArray, MeshDescriptor* descriptors, int meshCount, Mesh* d_meshes, LinearBVHNode* nodes, int numObjects);
__global__ void destroyWorld(Hittable** d_List, HittableList* d_World, Mesh* d_meshes);
#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <cuda_runtime.h>

#include "utils/utils.h"
#include "ray/ray.h"
#include "hittablelist/hittablelist.h"
#include "camera/camera.h"

class Framebuffer {
private:
	int width;
	int height;
	glm::vec3* d_Fb = nullptr;

public:
	__host__ Framebuffer(int width, int height) : width(width), height(height) {};
	__host__ ~Framebuffer() { cleanup(); };

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	__host__ void initialize();
	__host__ void cleanup();
	__device__ void writePixel(int x, int y, const glm::vec3& color);
	__device__ glm::vec3 color(const Ray& ray, HittableList* world, utils::random::RNG& rng);
	__device__ glm::vec3 colorPixel(int i, int j, int nx, int ny, Camera* camera, HittableList* world, utils::random::RNG& rng);
	__device__ glm::vec3* getDeviceBuffer() const { return d_Fb; };
	__host__ __device__ int getWidth() const { return width; };
	__host__ __device__ int getHeight() const { return height; };
	__host__ __device__ glm::vec3* getDeviceBuffer() const { return d_Fb; };

};

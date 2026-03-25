#pragma once

#include <cuda/std/optional>
#include <cuda_runtime.h>
#include <glm/glm.hpp>

#include "hittable/hittable.h"

class Mesh {
protected:
	glm::vec3* vertexArray = nullptr;
	int* indexArray = nullptr;
	int triangleCount = 0;
	Material* material = nullptr;
	glm::vec3 center{};
public:
	__host__ __device__ Mesh() = default;
	__host__ __device__ Mesh(glm::vec3* vertices, int* indices, int n, Material* m) : vertexArray(vertices), indexArray(indices), triangleCount(n), material(m) {};
	__host__ __device__ ~Mesh() = default;
	__device__ cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const;
	__device__ void setMaterial(Material* mat);
	__device__ Material* getMaterial() const;
};


struct MeshDescriptor {
	int vertexOffset;
	int indexOffset;
	int triangleCount;
	Material* material = nullptr;
};
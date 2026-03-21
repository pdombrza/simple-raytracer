#pragma once

#include <cuda/std/optional>
#include <cuda_runtime.h>
#include <glm/glm.hpp>

#include "hittable/hittable.h"

class Mesh : public Hittable {
protected:
	Triangle** triangles = nullptr;
	size_t triangleCount = 0;
	Material* material = nullptr;
	glm::vec3 center{};
public:
	__device__ Mesh(Triangle** triangles, size_t n, Material* m) : triangleCount(n), material(m) {
		this->triangles = triangles;
		glm::vec3 centerSum{};
		for (size_t i = 0; i < triangleCount; i++) {
			centerSum += triangles[i]->getCenter();
		}
		center = centerSum / (float)triangleCount;
	};

	__device__ ~Mesh();
	__device__ virtual cuda::std::optional<HitRecord> hit(const Ray& ray, float rayTMin, float rayTMax) const override;
	__device__ virtual HitRecord constructHitRecord(const Ray& ray, float t) const override;
	__device__ virtual void setMaterial(Material* mat) override;
	__device__ virtual Material* getMaterial() const override;
	__device__ virtual glm::vec3 getCenter() const override;
};
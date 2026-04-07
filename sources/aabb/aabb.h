#pragma once

#include <glm/glm.hpp>
#include <cuda/std/optional>
#include <cuda_runtime.h>

#include "utils/utils.h"
#include "hitrec/hitrec.h"

class AABB {
private:
	glm::vec3 bounds[2] = {
		glm::vec3(INF, INF, INF), // min
		glm::vec3(-INF, -INF, -INF)  // max
	};
public:
	__host__ __device__ AABB() = default;
	__host__ __device__ AABB(const glm::vec3& min, const glm::vec3& max) {
		bounds[0] = min;
		bounds[1] = max;
	};
	__host__ __device__ ~AABB() = default;
	__device__ bool hit(const Ray& ray, float rayTMin, float rayTMax) const {
		for (int axis = 0; axis < 3; axis++) {
			float t0 = (bounds[0][axis] - ray.getOrigin()[axis]) * ray.getInvDirection()[axis];
			float t1 = (bounds[1][axis] - ray.getOrigin()[axis]) * ray.getInvDirection()[axis];
			if (invD < 0.0f) {
				std::swap(t0, t1);
			}
			rayTMin = fmaxf(t0, rayTMin);
			rayTMax = fminf(t1, rayTMax);
			if (rayTMax <= rayTMin) {
				return false;
			}
		}
		return true;
	};

	__host__ __device__ void expand(const AABB& other) {
		bounds[0].x = fminf(bounds[0].x, other.bounds[0].x);
		bounds[0].y = fminf(bounds[0].y, other.bounds[0].y);
		bounds[0].z = fminf(bounds[0].z, other.bounds[0].z);

		bounds[1].x = fmaxf(bounds[1].x, other.bounds[1].x);
		bounds[1].y = fmaxf(bounds[1].y, other.bounds[1].y);
		bounds[1].z = fmaxf(bounds[1].z, other.bounds[1].z);
	};

	__host__ __device__ glm::vec3 getMin() const { return bounds[0]; };
	__host__ __device__ glm::vec3 getMax() const { return bounds[1]; };
	__host__ __device__ void setMin(const glm::vec3& newMin) { bounds[0] = newMin; };
	__host__ __device__ void setMax(const glm::vec3& newMax) { bounds[1] = newMax; };
};
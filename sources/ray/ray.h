#pragma once
#include <glm/glm.hpp>
#include <cuda_runtime.h>

#include "utils/utils.h"
#include "cuvec/cuvec.h"


class Ray {
private:
	cu::vec3 origin{ 0.0f, 0.0f, 0.0f };
	cu::vec3 direction{ 0.0f, 0.0f, 0.0f };
public:
	__device__ constexpr Ray() = default;
	__device__ Ray(const cu::vec3& origin, const cu::vec3& direction) : origin(origin), direction(cu::normalize(direction)) {};
	__device__ cu::vec3 getOrigin() const;
	__device__ cu::vec3 getDirection() const;
	__device__ void setDirection(const cu::vec3& newDir);
	__device__ void setOrigin(const cu::vec3& newOrigin);
	__device__ cu::vec3 At(const float t) const;
};	
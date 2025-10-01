#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "ray/ray.h"
#include "utils/utils.h"


struct ScatteringRecord {
	__device__ Ray ray{};
	__device__ cu::vec3 attenuation{ 0.0f, 0.0f, 0.0f };
};
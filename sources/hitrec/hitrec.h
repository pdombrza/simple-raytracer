#pragma once

#include <glm/glm.hpp>
#include "ray.h"


struct HitRecord {
	glm::vec3 p{};
	glm::vec3 normal{};
	float t;
};
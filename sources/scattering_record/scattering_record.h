#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "ray.h"
#include "utils.h"


struct ScatteringRecord {
	Ray ray{};
	glm::vec3 attenuation{ 0.0f, 0.0f, 0.0f };
};
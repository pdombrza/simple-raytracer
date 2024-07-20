#pragma once

#include <glm/glm.hpp>
#include "ray.h"
#include "utils.h"


struct HitRecord {
	glm::vec3 p{};
	glm::vec3 normal{};
	float t;
	bool frontFace;

	void setFaceNormal(const Ray& ray, const glm::vec3 outwardNormal) {
		// outwardNormal is supposed to be normalized
		frontFace = glm::dot(ray.getDirection(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};
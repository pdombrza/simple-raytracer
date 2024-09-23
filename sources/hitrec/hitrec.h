#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "ray.h"
#include "utils.h"
#include "scattering_record.h"


struct HitRecord {
	glm::vec3 p{};
	glm::vec3 normal{};
	float t;
	bool frontFace;

	void setFaceNormal(const Ray& ray, const glm::vec3& outwardNormal) {
		// outwardNormal is supposed to be normalized
		frontFace = glm::dot(ray.getDirection(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};


struct HitScatterRecord {
	std::optional<HitRecord> hitRec = std::nullopt;
	std::optional<ScatteringRecord> scatterRec = std::nullopt;
};
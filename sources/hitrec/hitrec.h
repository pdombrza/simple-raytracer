#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "ray/ray.h"
#include "cuvec/cuvec.h"
#include "utils/utils.h"
#include "scattering_record/scattering_record.h"


struct HitRecord {
	__device__ cu::vec3 p{};
	__device__ cu::vec3 normal{};
	__device__ float t;
	__device__ bool frontFace;

	__device__ void setFaceNormal(const Ray& ray, const cu::vec3& outwardNormal) {
		// outwardNormal is supposed to be normalized
		//frontFace = glm::dot(ray.getDirection(), outwardNormal) < 0;
		//normal = frontFace ? outwardNormal : -outwardNormal;
		return;
	}
};


struct HitScatterRecord {
	std::optional<HitRecord> hitRec = std::nullopt;
	std::optional<ScatteringRecord> scatterRec = std::nullopt;
};
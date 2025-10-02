#pragma once

#include <memory>
#include <cuda_runtime.h>
#include <cuda/std/optional>
#include "ray/ray.h"
#include "cuvec/cuvec.h"
#include "utils/utils.h"
#include "scattering_record/scattering_record.h"


struct HitRecord {
	cu::vec3 p{};
	cu::vec3 normal{};
	float t;
	bool frontFace;

	__device__ void setFaceNormal(const Ray& ray, const cu::vec3& outwardNormal) {
		// outwardNormal is supposed to be normalized
		frontFace = cu::dot(ray.getDirection(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : outwardNormal * -1.0f;
		return;
	}
};


struct HitScatterRecord {
	cuda::std::optional<HitRecord> hitRec = cuda::std::nullopt;
	cuda::std::optional<ScatteringRecord> scatterRec = cuda::std::nullopt;
};
#pragma once

#include <cuda_runtime.h>
#include "ray/ray.h"
#include "utils/utils.h"


struct ScatteringRecord {
	Ray ray{};
	cu::vec3 attenuation{ 0.0f, 0.0f, 0.0f };
};
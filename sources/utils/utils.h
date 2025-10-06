#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <curand_kernel.h>
#include <utility>
#include <optional>

#include <cuda/std/limits>
#include <device_launch_parameters.h>

#include <glm/glm.hpp>

#define INF cuda::std::numeric_limits<float>::max()


namespace utils {
	namespace random {
		__global__ void randomInit(curandState* randStates, int x, int y);
		template<typename T>
		__device__ T getRandom(curandState* state, T min, T max);
		__device__ float getRandomNorm(curandState* state);
		__device__ glm::vec3 sampleSquare(curandState* state);
		template<typename T>
		__device__ glm::vec<3, T, glm::defaultp> randomVec3(curandState* state, T min, T max);
		__device__ glm::vec3 randomVec3Norm(curandState* state);
		__device__ glm::vec3 randomVec3InSphere(curandState* state);
		__device__ glm::vec3 randomVec3OnHemisphere(curandState* state, const glm::vec3& normal);
		__device__ glm::vec3 randomVec3InDisk(curandState* state);
	}
}

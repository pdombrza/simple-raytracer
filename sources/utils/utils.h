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
		class RNG {
		private:
			curandState* state;
		public:
			__device__ explicit RNG(curandState* state) : state(state) {};
			__device__ ~RNG() = default;
			template<typename T>
			__device__ T getRandom(T min, T max);
			__device__ float getRandomUniform();
			__device__ glm::vec3 sampleSquare();
			template<typename T>
			__device__ glm::vec<3, T, glm::defaultp> randomVec3(T min, T max);
			__device__ glm::vec3 randomVec3Norm();
			__device__ glm::vec3 randomVec3InSphere();
			__device__ glm::vec3 randomVec3OnHemisphere(const glm::vec3& normal);
			__device__ glm::vec3 randomVec3InDisk();
		};
		
		__global__ void randomInit(curandState* randStates, int x, int y);
	}
}

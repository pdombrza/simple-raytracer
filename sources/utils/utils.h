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
		__global__ void randomInit(curandState* randStates, int x, int y) {
			int i = threadIdx.x + blockIdx.x * blockDim.x;
			int j = threadIdx.y + blockIdx.y * blockDim.y;
			if ((i >= x) || (j >= y)) return;
			int pixelIdx = j * x + i;
			curand_init(1984, pixelIdx, 0, &randStates[pixelIdx]); // literally brave new world
		}

		template<typename T>
		__device__ inline T getRandom(curandState* state, T min, T max) {
			float generatedRandom = curand_uniform(state);
			return static_cast<float>min + (static_cast<float>max - static_cast<float>min) * generatedRandom;
		}

		__device__ inline float getRandomNorm(curandState* state) {
			return curand_uniform(state);
		}

		__device__ inline glm::vec3 sampleSquare(curandState* state) {
			return glm::vec3(curand_uniform(state) - 0.5f, curand_uniform(state) - 0.5f, 0.0f);
		}

		template<typename T> // this template now becomes useless since randoms will always be float
		__device__ inline glm::vec<3, T, glm::defaultp> randomVec3(curandState* state, T min, T max) {
			return glm::vec<3, T, glm::defaultp>(getRandom(state, min, max), getRandom(state, min, max), getRandom(state, min, max));
		}

		__device__ inline glm::vec3 randomVec3Norm(curandState* state) {
			glm::vec3 res = randomVec3(state, 0.0f, 1.0f);
			return res;
		}

		inline glm::vec3 randomVec3InSphere() {
			while (true) {
				glm::vec3 randomVec = (randomVec3Norm() - 0.5f) * 2.0f;
				if (std::pow(glm::length(randomVec), 2) < 1.0f)
					return glm::normalize(randomVec);
			}
		}

		inline glm::vec3 randomVec3OnHemisphere(const glm::vec3& normal) {
			glm::vec3 onUnitSphere = randomVec3InSphere();
			onUnitSphere = (glm::dot(onUnitSphere, normal) > 0.0f) ? onUnitSphere : -onUnitSphere;
			return onUnitSphere;
		}

		inline glm::vec3 randomVec3InDisk() {
			while (true) {
				glm::vec3 randomVec{ getRandom(-1.0f, 1.0f), getRandom(-1.0f, 1.0f), 0.0f };
				if (std::pow(glm::length(randomVec), 2) < 1.0f)
					return randomVec;
			}
		}
	}
}

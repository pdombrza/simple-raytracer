#include "utils.h"

__global__ void utils::random::randomInit(curandState* randStates, int x, int y) {
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	if ((i >= x) || (j >= y)) return;
	int pixelIdx = j * x + i;
	curand_init(1984, pixelIdx, 0, &randStates[pixelIdx]); // literally brave new world
}

template<typename T>
__device__ T utils::random::getRandom(curandState* state, T min, T max) {
	float generatedRandom = curand_uniform(state);
	return static_cast<float>(min) + (static_cast<float>(max) - static_cast<float>(min)) * generatedRandom;
}

__device__ float utils::random::getRandomNorm(curandState* state) {
	return curand_uniform(state);
}

__device__ glm::vec3 utils::random::sampleSquare(curandState* state) {
	return glm::vec3(curand_uniform(state) - 0.5f, curand_uniform(state) - 0.5f, 0.0f);
}

template<typename T> // this template now becomes useless since randoms will always be float
__device__ glm::vec<3, T, glm::defaultp> utils::random::randomVec3(curandState* state, T min, T max) {
	return glm::vec<3, T, glm::defaultp>(getRandom(state, min, max), getRandom(state, min, max), getRandom(state, min, max));
}

__device__ glm::vec3 utils::random::randomVec3Norm(curandState* state) {
	glm::vec3 res = randomVec3(state, 0.0f, 1.0f);
	return res;
}

__device__ glm::vec3 utils::random::randomVec3InSphere(curandState* state) {
	while (true) {
		glm::vec3 randomVec = (randomVec3Norm(state) - 0.5f) * 2.0f;
		if (glm::dot(randomVec, randomVec) < 1.0f)
			return glm::normalize(randomVec);
	}
}

__device__ glm::vec3 utils::random::randomVec3OnHemisphere(curandState* state, const glm::vec3& normal) {
	glm::vec3 onUnitSphere = randomVec3InSphere(state);
	onUnitSphere = (glm::dot(onUnitSphere, normal) > 0.0f) ? onUnitSphere : -onUnitSphere;
	return onUnitSphere;
}

__device__ glm::vec3 utils::random::randomVec3InDisk(curandState* state) {
	while (true) {
		glm::vec3 randomVec{ getRandom(state, -1.0f, 1.0f), getRandom(state, -1.0f, 1.0f), 0.0f };
		if (glm::dot(randomVec, randomVec) < 1.0f)
			return randomVec;
	}
}
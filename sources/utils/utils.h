#pragma once

#include <cmath>
#include <random>

#include <glm/glm.hpp>

#define INF std::numeric_limits<float>::max()

namespace Utils::random {
	template<typename T>
	inline T getRandom(T min, T max) {
		static std::random_device rd;
		static std::mt19937 generator(rd());
		static std::uniform_real_distribution<T> dist(min, max);
		return dist(generator);
	}

	inline float getRandomNorm() {
		return getRandom(0.0f, 1.0f);
	}

	inline glm::vec3 sampleSquare() {
		return glm::vec3(getRandomNorm() - 0.5f, getRandomNorm() - 0.5f, 0.0f);
	}

	template<typename T>
	inline glm::vec<3, T, glm::defaultp> randomVec3(T min, T max) {
		return glm::vec<3, T, glm::defaultp>(getRandom(min, max), getRandom(min, max), getRandom(min, max));
	}

	inline glm::vec3 randomVec3Norm() {
		glm::vec3 res = randomVec3(0.0f, 1.0f);
		return res;
	}
}
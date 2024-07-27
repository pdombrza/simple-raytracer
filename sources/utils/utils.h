#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <utility>
#include <optional>

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

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#include <glm/glm.hpp>

#define INF std::numeric_limits<float>::max()


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
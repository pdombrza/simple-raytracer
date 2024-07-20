#pragma once
#include <glm/glm.hpp>

#include "utils.h"


class Ray {
private:
	glm::vec3 origin{ 0.0f, 0.0f, 0.0f };
	glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
public:
	constexpr Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction) : origin{ origin }, direction{ glm::normalize(direction) } {};
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
	void setDirection(const glm::vec3& newDir);
	void setOrigin(const glm::vec3& newOrigin);
	glm::vec3 At(const float t) const;
};	
#pragma once
#include <glm/glm.hpp>


class Ray {
private:
	glm::vec3 origin{ 0.f, 0.f, 0.f };
	glm::vec3 direction{ 0.f, 0.f, 0.f };
public:
	constexpr Ray() = default;
	Ray(const glm::vec3& direction, const glm::vec3& origin) : origin{ origin }, direction{ direction } {};
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
	void setDirection(const glm::vec3& newDir);
	void setOrigin(const glm::vec3& newOrigin);
	glm::vec3 At(const float t) const;
};	
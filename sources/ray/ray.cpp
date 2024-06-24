#include <glm/glm.hpp>

#include "ray.h"


glm::vec3 Ray::getOrigin() const {
	return origin;
}

glm::vec3 Ray::getDirection() const {
	return direction;
}

void Ray::setDirection(const glm::vec3& newDir) {
	direction = newDir;
}

void Ray::setOrigin(const glm::vec3& newOrigin) {
	origin = newOrigin;
}

glm::vec3 Ray::At(const float t) const {
	return origin + direction * t;
}
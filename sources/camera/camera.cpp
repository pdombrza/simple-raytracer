#include "camera.h"

void Camera::initialize(int imgWidth, int imgHeight) {
	center = orientation.lookFrom;
	float theta = glm::radians(vFov);
	float h = std::tan(theta / 2);
	float viewportHeight = 2 * h * focusDist;
	float viewportWidth = viewportHeight * ((float)imgWidth / imgHeight);

	w = glm::normalize(orientation.lookFrom - orientation.lookAt);
	u = glm::normalize(glm::cross(orientation.vUp, w));
	v = glm::cross(w, u);


	glm::vec3 viewportU = viewportWidth * u;
	glm::vec3 viewportV = viewportHeight * (-v);

	pixelDeltaU = viewportU / (float)imgWidth;
	pixelDeltaV = viewportV / (float)imgHeight;
	
	glm::vec3 viewportUpperLeft = center - (focusDist * w) - viewportU / 2.0f - viewportV / 2.0f;
	startPixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

	// Camera defocus disk basis vectors calculations
	float defocusRadius = focusDist * std::tan(glm::radians(defocusAngle / 2.0f));
	defocusDiskU = u * defocusRadius;
	defocusDiskV = v * defocusRadius;
}

Ray Camera::getRay(float u, float v) const {
	glm::vec3 offset = Utils::random::sampleSquare();
	glm::vec3 pixelSample = startPixelLoc + (u + offset.x) * pixelDeltaU + (v + offset.y) * pixelDeltaV;
	glm::vec3 rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
	glm::vec3 rayDirection = pixelSample - rayOrigin;
	return Ray(rayOrigin, rayDirection);
}

glm::vec3 Camera::defocusDiskSample() const {
	glm::vec3 p = Utils::random::randomVec3InDisk();
	return center + p.x * defocusDiskU + p.y * defocusDiskV;
}
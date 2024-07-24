#include "camera.h"

void Camera::initialize(int imgWidth, int imgHeight) {
	center = orientation.lookFrom;
	float focalLength = (orientation.lookFrom - orientation.lookAt).length();
	float theta = glm::radians(vFov);
	float h = std::tan(theta / 2);
	float viewportHeight = 2 * h * focalLength;
	float viewportWidth = viewportHeight * ((float)imgWidth / imgHeight);

	w = glm::normalize(orientation.lookFrom - orientation.lookAt);
	u = glm::normalize(glm::cross(orientation.vUp, w));
	v = glm::cross(w, u);


	glm::vec3 viewportU = viewportWidth * u;
	glm::vec3 viewportV = viewportHeight * (-v);

	pixelDeltaU = viewportU / (float)imgWidth;
	pixelDeltaV = viewportV / (float)imgHeight;

	glm::vec3 viewportUpperLeft = center - (focalLength * w) - viewportU / 2.0f - viewportV / 2.0f;
	startPixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
}

Ray Camera::getRay(float u, float v) const {
	glm::vec3 offset = Utils::random::sampleSquare();
	glm::vec3 pixelSample = startPixelLoc + (u + offset.x) * pixelDeltaU + (v + offset.y) * pixelDeltaV;
	glm::vec3 rayOrigin = center;
	glm::vec3 rayDirection = pixelSample - rayOrigin;
	return Ray(rayOrigin, rayDirection);
}
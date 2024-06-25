#include "camera.h"

void Camera::initialize(int imgWidth, int imgHeight) {
	float viewportWidth = viewportHeight * ((float)imgWidth / imgHeight);

	glm::vec3 viewportU(viewportWidth, 0.0f, 0.0f);
	glm::vec3 viewportV(0.0f, -viewportHeight, 0.0f);

	pixelDeltaU = viewportU / (float)imgWidth;
	pixelDeltaV = viewportV / (float)imgHeight;

	glm::vec3 viewportUpperLeft = center - glm::vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
	startPixelLoc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
}
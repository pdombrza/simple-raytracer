#pragma once

#include <glm/glm.hpp>

#include "hittable.h"
#include "hitrec.h"
#include "ray.h"
#include "utils.h"


class Camera {
private:
	float aspectRatio = 16.0f / 9.0f;
	float focalLength = 1.0f;
	float viewportHeight = 2.0f;
	glm::vec3 center{ 0.0f, 0.0f, 0.0f };
	glm::vec3 startPixelLoc{};
	glm::vec3 pixelDeltaU{};
	glm::vec3 pixelDeltaV{};
public:
	Camera() {};
	Camera(float aspectRatio) : aspectRatio(aspectRatio) {};
	Camera(float aspectRatio, float focalLength, float viewportHeight, const glm::vec3& center) : aspectRatio(aspectRatio), focalLength(focalLength), viewportHeight(viewportHeight), center(center) {};
	~Camera() = default;
	void initialize(int imgWidth, int imgHeight);
	Ray getRay(float h, float w) const;
	void setAspectRatio(float newAspectRatio) { aspectRatio = newAspectRatio; };
	float getAspectRatio() const { return aspectRatio; };
	void setViewportHeight(float newViewportHeight) { viewportHeight = newViewportHeight; };
	float getViewportHeight() const { return viewportHeight; };
	void setFocalLength(float newFocalLength) { focalLength = newFocalLength; };
	float getFocalLength() const { return focalLength; };
	void setCenter(glm::vec3& newCameraCenter) { center = newCameraCenter; };
	glm::vec3 getCenter() const { return center; };
	glm::vec3 getStartPixelLoc() const { return startPixelLoc; };
	glm::vec3 getPixelDeltaU() const { return pixelDeltaU; };
	glm::vec3 getPixelDeltaV() const { return pixelDeltaV; };
};
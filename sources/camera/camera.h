#pragma once

#include <glm/glm.hpp>

#include "hittable.h"
#include "hitrec.h"
#include "ray.h"
#include "utils.h"


struct CameraOrientation {
	glm::vec3 lookFrom{};
	glm::vec3 lookAt{ 0.0f, 0.0f, -1.0f };
	glm::vec3 vUp{ 0.0f, 1.0f, 0.0f }; // view up vector
};


class Camera {
private:
	float aspectRatio = 16.0f / 9.0f;
	float vFov = 90.0f; // in degrees
	glm::vec3 center{};
	glm::vec3 startPixelLoc{};
	glm::vec3 pixelDeltaU{};
	glm::vec3 pixelDeltaV{};
	glm::vec3 u{}, v{}, w{};
	CameraOrientation orientation{};
public:
	Camera() {};
	Camera(CameraOrientation orientation) : orientation(orientation) {};
	Camera(float aspectRatio) : aspectRatio(aspectRatio) {};
	//Camera(float vFov) : vFov(vFov) {};
	Camera(float vFov, float aspectRatio) : vFov(vFov), aspectRatio(aspectRatio) {};
	Camera(CameraOrientation orientation, float vFov, float aspectRatio) : orientation(orientation), vFov(vFov), aspectRatio(aspectRatio) {};
	Camera(float aspectRatio, float focalLength, float viewportHeight, const glm::vec3& center) : aspectRatio(aspectRatio), center(center) {};
	~Camera() = default;
	void initialize(int imgWidth, int imgHeight);
	Ray getRay(float h, float w) const;
	void setAspectRatio(float newAspectRatio) { aspectRatio = newAspectRatio; };
	float getAspectRatio() const { return aspectRatio; };
	void setCenter(glm::vec3& newCameraCenter) { center = newCameraCenter; };
	float getVFov() const { return vFov; };
	void setVFov(float newVFov) { vFov = newVFov; };
	CameraOrientation getCameraOrientation() const { return orientation; };
	void setCameraOrientation(CameraOrientation& newOrientation) { orientation = newOrientation; };
	glm::vec3 getCenter() const { return center; };
	glm::vec3 getStartPixelLoc() const { return startPixelLoc; };
	glm::vec3 getPixelDeltaU() const { return pixelDeltaU; };
	glm::vec3 getPixelDeltaV() const { return pixelDeltaV; };
};
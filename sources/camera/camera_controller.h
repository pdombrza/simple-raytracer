#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "camera/camera.h"

class CameraController {
private:
	CameraOrientation& orientation;

	float yaw{};
	float pitch{};
	float movementSpeed{};
	float mouseSensitivity{};
	bool changed = false;

	void updateOrientation();
public:
	CameraController(CameraOrientation& orientation, float speed = 2.5f, float sensitivity = 0.1f) : orientation(orientation), movementSpeed(speed), mouseSensitivity(sensitivity) {
		glm::vec3 dir = glm::normalize(orientation.lookAt - orientation.lookFrom);
		pitch = glm::degrees(asin(dir.y));
		yaw = glm::degrees(atan2(dir.z, dir.x));
	};
	bool isChanged() const { return changed; }
	void clearChanged() { changed = false; }
	CameraOrientation getOrientation() const { return orientation; }

	void processMouse(float xOffset, float yOffset);
	void handleInputs(GLFWwindow* window, float deltaTime);
};
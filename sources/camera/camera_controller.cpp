#include "camera_controller.h"

#include "CameraUIControl.h"

void CameraController::handleInputs(GLFWwindow * window, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    glm::vec3 forward = glm::normalize(orientation.lookAt - orientation.lookFrom);
    glm::vec3 right = glm::normalize(glm::cross(forward, orientation.vUp));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));

    glm::vec3 movement{};
    bool moved = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { movement += forward * velocity; moved = true; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { movement -= forward * velocity; moved = true; }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { movement -= right * velocity;   moved = true; }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { movement += right * velocity;   moved = true; }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { movement += up * velocity;   moved = true; }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { movement -= up * velocity;   moved = true; }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (moved) {
        orientation.lookFrom += movement;
        orientation.lookAt += movement;
        changed = true;
    }
}

void CameraController::processMouse(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateOrientation();
    changed = true;
}

void CameraController::updateOrientation() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    orientation.lookAt = orientation.lookFrom + glm::normalize(dir);
}
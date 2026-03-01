#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Camera::Camera(float radius) : radius(radius) {}

glm::mat4 Camera::getView() const {
    glm::vec3 pos = {
        target.x + radius * cos(pitch) * sin(yaw),
        target.y + radius * sin(pitch),
        target.z + radius * cos(pitch) * cos(yaw)
    };
    return glm::lookAt(pos, target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjection(float aspect) const {
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
}

void Camera::processInput(GLFWwindow* window, float dt) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    bool leftDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (leftDown && !firstMouse) {
        float dx = (float)(mx - lastMouseX) * 0.005f;
        float dy = (float)(my - lastMouseY) * 0.005f;
        yaw   += dx;
        pitch  = std::clamp(pitch - dy, -1.5f, 1.5f);
    }

    firstMouse = false;
    lastMouseX = mx;
    lastMouseY = my;

    // Scroll zoom via Q/E keys
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) radius += 5.0f * dt;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) radius = std::max(1.0f, radius - 5.0f * dt);
}
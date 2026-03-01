#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(float radius = 10.0f);

    glm::mat4 getView() const;
    glm::mat4 getProjection(float aspect) const;
    void processInput(GLFWwindow* window, float dt);

    float yaw = 0.0f, pitch = 0.3f, radius = 10.0f;
    glm::vec3 target = glm::vec3(0.0f);

private:
    double lastMouseX = 0, lastMouseY = 0;
    bool firstMouse = true;
    bool dragging = false;
};
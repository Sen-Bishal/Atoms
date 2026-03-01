#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine();

    bool shouldClose() const;
    void beginFrame();
    void endFrame();
    GLFWwindow* getWindow() const { return window; }

    int width, height;

private:
    GLFWwindow* window;

    static void framebufferSizeCallback(GLFWwindow* w, int width, int height);
};
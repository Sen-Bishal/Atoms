#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Upload a cloud of 3D points and draw them
    void drawPointCloud(const std::vector<glm::vec3>& points,
                        const std::vector<glm::vec3>& colors,
                        const glm::mat4& view,
                        const glm::mat4& proj);

    void drawGrid(const glm::mat4& view, const glm::mat4& proj);

private:
    GLuint pointShader;
    GLuint gridShader;
    GLuint pointVAO, pointVBO, colorVBO;
    GLuint gridVAO, gridVBO;
    int gridLineCount = 0;

    GLuint compileShader(const char* vertPath, const char* fragPath);
    void buildGrid(float size, int divisions);
};
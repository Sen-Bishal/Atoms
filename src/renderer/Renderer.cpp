#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

static std::string readFile(const char* path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error(std::string("Cannot open shader: ") + path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static GLuint compileShaderSrc(GLenum type, const std::string& src) {
    GLuint s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);
    int ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512]; glGetShaderInfoLog(s, 512, nullptr, log);
        throw std::runtime_error(std::string("Shader error: ") + log);
    }
    return s;
}

Renderer::Renderer() {
    // Point cloud shader
    auto vSrc = readFile("shaders/point.vert");
    auto fSrc = readFile("shaders/point.frag");
    GLuint vs = compileShaderSrc(GL_VERTEX_SHADER, vSrc);
    GLuint fs = compileShaderSrc(GL_FRAGMENT_SHADER, fSrc);
    pointShader = glCreateProgram();
    glAttachShader(pointShader, vs); glAttachShader(pointShader, fs);
    glLinkProgram(pointShader);
    glDeleteShader(vs); glDeleteShader(fs);

    // Point cloud VAO
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);
    glGenBuffers(1, &colorVBO);

    // Grid shader (reuse same shader for now)
    gridShader = pointShader;

    // Build grid
    buildGrid(20.0f, 20);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &pointVAO);
    glDeleteBuffers(1, &pointVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteProgram(pointShader);
}

void Renderer::drawPointCloud(const std::vector<glm::vec3>& points,
                               const std::vector<glm::vec3>& colors,
                               const glm::mat4& view,
                               const glm::mat4& proj)
{
    glUseProgram(pointShader);
    glUniformMatrix4fv(glGetUniformLocation(pointShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(pointShader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(pointVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_POINTS, 0, (GLsizei)points.size());
    glDisable(GL_BLEND);

    glBindVertexArray(0);
}

void Renderer::drawGrid(const glm::mat4& view, const glm::mat4& proj) {
    glUseProgram(gridShader);
    glUniformMatrix4fv(glGetUniformLocation(gridShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(gridShader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, (GLsizei)(gridLineCount));
    glBindVertexArray(0);
}

void Renderer::buildGrid(float size, int divisions) {
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> cols;
    float step = size / divisions;
    glm::vec3 col(0.3f, 0.3f, 0.3f);

    for (int i = -divisions; i <= divisions; i++) {
        float t = i * step;
        verts.push_back({t, 0, -size}); cols.push_back(col);
        verts.push_back({t, 0,  size}); cols.push_back(col);
        verts.push_back({-size, 0, t}); cols.push_back(col);
        verts.push_back({ size, 0, t}); cols.push_back(col);
    }
    gridLineCount = (int)verts.size();

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    GLuint colorBuf;
    glGenBuffers(1, &colorBuf);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuf);
    glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(glm::vec3), cols.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
}
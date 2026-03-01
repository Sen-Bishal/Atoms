#include "engine/Engine.h"
#include "engine/Camera.h"
#include "renderer/Renderer.h"
#include "physics/QMSolver.h"
#include <vector>
#include <glm/glm.hpp>

int main() {
    Engine engine(1280, 720, "Quantum Atom Simulator");
    Camera camera(10.0f);
    Renderer renderer;

    // Initial quantum state: hydrogen ground state n=1, l=0, m=0
    QNumbers q = {1, 0, 0};
    const int NUM_PARTICLES = 50000;

    std::vector<glm::vec3> positions, colors;
    positions.reserve(NUM_PARTICLES);
    colors.reserve(NUM_PARTICLES);

    // Sample orbital positions from probability distribution
    for (int i = 0; i < NUM_PARTICLES; i++) {
        glm::vec3 pos = QMSolver::sampleOrbitalPosition(q);
        positions.push_back(pos);
        colors.push_back(glm::vec3(0.2f, 0.6f, 1.0f)); // blue electrons
    }

    float lastTime = 0.0f;

    while (!engine.shouldClose()) {
        float now = (float)glfwGetTime();
        float dt  = now - lastTime;
        lastTime  = now;

        camera.processInput(engine.getWindow(), dt);

        engine.beginFrame();

        float aspect = (float)engine.width / engine.height;
        auto view = camera.getView();
        auto proj = camera.getProjection(aspect);

        renderer.drawGrid(view, proj);
        renderer.drawPointCloud(positions, colors, view, proj);

        engine.endFrame();

        // Keyboard: change quantum numbers on the fly
        auto* win = engine.getWindow();
        static bool nPressed = false;
        if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS && !nPressed) {
            if (q.n < 6) { q.n++; q.l = 0; q.m = 0; }
            positions.clear(); colors.clear();
            for (int i = 0; i < NUM_PARTICLES; i++) {
                positions.push_back(QMSolver::sampleOrbitalPosition(q));
                colors.push_back(glm::vec3(0.2f, 0.6f, 1.0f));
            }
            nPressed = true;
        }
        if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_RELEASE) nPressed = false;
    }

    return 0;
}
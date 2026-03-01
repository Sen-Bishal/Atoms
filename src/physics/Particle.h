#pragma once
#include <glm/glm.hpp>

enum class ParticleType { Proton, Neutron, Electron };

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    ParticleType type;
    int energyLevel = 1;       // n
    int orbitalShape = 0;      // l
    int orbitalRotation = 0;   // m
    float excitedTimer = 0.0f;

    glm::vec3 getColor() const;
};
#include "Particle.h"

glm::vec3 Particle::getColor() const {
    switch(type) {
        case ParticleType::Proton:   return {1.0f, 0.3f, 0.3f};
        case ParticleType::Neutron:  return {0.6f, 0.6f, 0.6f};
        case ParticleType::Electron: return {0.2f, 0.6f, 1.0f};
    }
    return {1,1,1};
}
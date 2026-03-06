#version 330 core

// Per-vertex (unit icosphere)
layout(location = 0) in vec3 aPos;

// Per-instance
layout(location = 1) in vec3  iWorldPos;
layout(location = 2) in vec3  iColor;      // precomputed base colour (density-mapped)
layout(location = 3) in float iDensity;    // raw density t ∈ [0,1] for colorScale remap

uniform mat4  view;
uniform mat4  proj;
uniform float uRadius;

out vec3  vNormal;
out vec3  vFragPos;
out vec3  vColor;
out float vDensity;

// Flat — same value for every fragment in the same sphere instance
flat out vec3 vInstancePos;

void main() {
    vec3 worldPos = aPos * uRadius + iWorldPos;

    gl_Position  = proj * view * vec4(worldPos, 1.0);
    vNormal      = aPos;
    vFragPos     = worldPos;
    vColor       = iColor;
    vDensity     = iDensity;
    vInstancePos = iWorldPos;
}

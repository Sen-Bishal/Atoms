#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 proj;

out vec3 fragColor;

void main() {
    gl_Position  = proj * view * vec4(aPos, 1.0);
    gl_PointSize = max(1.0, 3.0 / gl_Position.w); // perspective-aware point size
    fragColor    = aColor;
}
#version 330 core
in vec3 fragColor;
out vec4 outColor;

void main() {
    // Soft circular point
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);
    if (dist > 0.5) discard;
    float alpha = 1.0 - smoothstep(0.3, 0.5, dist);
    outColor = vec4(fragColor, alpha * 0.6);
}
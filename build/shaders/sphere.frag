#version 330 core

in vec3  vNormal;
in vec3  vFragPos;
in vec3  vColor;
in float vDensity;
flat in vec3 vInstancePos;

out vec4 outColor;

uniform vec3  uLightDir;     // world-space, pre-normalised
uniform vec3  uViewPos;      // camera world position
uniform float uColorScale;   // density contrast: 1.0 = default, >1 = more vivid
uniform vec3  uClip;         // per-axis clip plane; value >= 98 means disabled

// Density → colour ramp (mirrors the CPU version so colorScale remaps it live)
vec3 densityColor(float t) {
    vec3 sparse = vec3(0.4, 0.0, 0.6);
    vec3 mid    = vec3(0.8, 0.3, 0.2);
    vec3 dense  = vec3(1.0, 0.6, 0.0);
    if (t < 0.5)
        return mix(sparse, mid,   t * 2.0);
    else
        return mix(mid,    dense, (t - 0.5) * 2.0);
}

void main() {
    // ---- Axis clipping (whole-sphere: test instance centre, not fragment) ----
    if (uClip.x < 98.0 && vInstancePos.x > uClip.x) discard;
    if (uClip.y < 98.0 && vInstancePos.y > uClip.y) discard;
    if (uClip.z < 98.0 && vInstancePos.z > uClip.z) discard;

    // ---- Colour scale: remap density before colouring ----
    // Raising t to (1/colorScale) pushes sparse regions toward the bright end.
    float t = pow(clamp(vDensity, 0.001, 1.0), 1.0 / max(uColorScale, 0.1));
    vec3 baseColor = densityColor(t);

    // ---- Blinn-Phong lighting ----
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightDir);
    vec3 V = normalize(uViewPos - vFragPos);
    vec3 H = normalize(L + V);

    float ambient  = 0.18;
    float diffuse  = max(dot(N, L), 0.0) * 0.72;
    float specular = pow(max(dot(N, H), 0.0), 48.0) * 0.55;

    float rim = 1.0 - max(dot(N, V), 0.0);
    rim = pow(rim, 3.0) * 0.15;

    vec3 col = baseColor * (ambient + diffuse) + vec3(specular) - vec3(rim);
    outColor = vec4(max(col, vec3(0.0)), 1.0);
}

# Quantum Atom Simulator — Development Roadmap

---

## Phase 1 — Visual Polish

**Goal:** Make the current point cloud look and feel interactive.

- [ ] Add `l` and `m` keyboard controls alongside the existing `n` control
  - `n` → Arrow Up / Down
  - `l` → Left / Right (clamped to `0 <= l < n`)
  - `m` → `[` / `]` (clamped to `-l <= m <= l`)
- [ ] Display current quantum numbers as a HUD overlay in the window title or via a simple text render
- [ ] Color points by local probability density
  - Denser regions → brighter/whiter
  - Sparse regions → darker/more transparent
  - Requires computing a local density estimate per point (e.g. count neighbors within radius)

---

## Phase 2 — Probability Current & Phase Flow

**Goal:** Animate the orbital so it looks alive, matching the flowing effect from the video.

- [ ] Implement the probability current velocity field
  - Velocity is derived from the gradient of the phase of the wave function `ψ`
  - For real orbitals, current flows azimuthally (around the z-axis) based on quantum number `m`
- [ ] Update each particle's `φ` coordinate each frame using its computed velocity
- [ ] Keep the overall density distribution unchanged — only the flow/motion is affected
- [ ] Expose a speed multiplier control to tune the animation rate

---

## Phase 3 — Ray Traced Look

**Goal:** Replicate the beautiful render style from the video's final segment.

- [ ] Replace `GL_POINTS` with small instanced sphere meshes
  - Use a low-poly icosphere (subdivided once or twice) per point for performance
  - Leverage `glDrawArraysInstanced` to render 50k+ spheres efficiently
- [ ] Build a local density field
  - For each point, count how many other points fall within a fixed radius
  - Store density as a per-point float attribute
- [ ] Color map points by density
  - Low density → deep blue / purple
  - High density → white / cyan (matching the video's color scheme)
- [ ] Add a basic lighting model to the instanced spheres (Phong or Blinn-Phong)
- [ ] Tune point size and transparency for a volumetric cloud feel

---

## Phase 4 — Photon & Energy Level System

**Goal:** Reintroduce the Bohr model photon absorption/emission from the early part of the video.

- [ ] Reintroduce the `Photon` struct with position, direction, energy, and wavelength
- [ ] Implement photon rendering as animated sine waves (as in the 2D version)
- [ ] Implement absorption logic
  - If a photon's energy matches the gap between two energy levels, it is absorbed
  - Electron jumps from `n` → `n+1` and enters an excited state with a timer
- [ ] Implement emission logic
  - When the excited timer expires, the electron drops back down
  - A new photon is emitted in a random direction with the correct energy/color
- [ ] Visually transition the orbital point cloud between states during a jump
  - Lerp or cross-fade between the two orbital distributions over ~0.5s
- [ ] Add mouse click control to spawn photons into the scene

---

## Stretch Goals

- [ ] Multi-atom scene — render a grid of atoms and watch photons propagate through them
- [ ] Entropy / blackbody simulation — replicate the Earth energy absorption demo from the video
- [ ] Export a frame as a high-res PNG for wallpaper-quality renders
- [ ] Web version using WebGL / Emscripten

---

## Current State

| System | Status |
|---|---|
| Window & OpenGL context | ✅ Done |
| 3D camera (orbit + zoom) | ✅ Done |
| Schrödinger equation solver | ✅ Done |
| CDF orbital sampling | ✅ Done |
| Point cloud renderer | ✅ Done |
| Grid | ✅ Done |
| n control (Arrow Up) | ✅ Done |
| l / m controls | ⬜ Phase 1 |
| Density coloring | ⬜ Phase 1 |
| Probability current flow | ⬜ Phase 2 |
| Instanced sphere renderer | ⬜ Phase 3 |
| Photon system | ⬜ Phase 4 |
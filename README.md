# Quantum Atom Simulator

A real-time 3D simulation of atomic orbitals, written in C++ using OpenGL. This project started as a curiosity about what atoms actually look like — not the cartoon solar system model everyone learns in school, but the real quantum mechanical picture.

The short answer is: a lot stranger, and a lot more beautiful.

---

## What it does

The simulator solves the Schrodinger equation for hydrogen-like atoms and renders the resulting electron probability distributions as interactive 3D point clouds. Each point represents a possible position the electron could occupy — the cloud as a whole is the orbital itself.

Quantum numbers can be changed at runtime, letting you explore how the shape of an orbital responds to different energy levels and angular momentum states.

---

## The physics

Electrons do not orbit the nucleus like planets around a sun. They exist as standing waves, and their position at any given moment is fundamentally probabilistic. The Schrodinger equation describes these waves mathematically, and its solutions — the wave functions — tell us the probability of finding an electron at any point in space.

This simulator computes those probabilities directly using:

- The radial wave function, built from associated Laguerre polynomials
- The angular wave function, built from spherical harmonics via associated Legendre polynomials
- CDF (Cumulative Distribution Function) sampling to turn probability distributions into actual 3D positions

The result is a point cloud that accurately reflects the true shape of each orbital.

---

## Stack

- C++20
- OpenGL 3.3 Core Profile
- GLFW — windowing and input
- GLAD — OpenGL loader
- GLM — mathematics

---

## Building

Requires Visual Studio Build Tools 2022 or later and vcpkg.

Install dependencies:
```
vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows
```

Configure and build:
```
mkdir build && cd build
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

---

## Controls

| Input | Action |
|---|---|
| Left mouse drag | Orbit camera |
| Q / E | Zoom out / in |
| Arrow Up / Down | Increase / decrease principal quantum number n |
| Arrow Left / Right | Adjust orbital shape quantum number l |
| [ / ] | Adjust magnetic quantum number m |

---

## Quantum numbers

Each orbital is defined by three quantum numbers that determine its shape, size, and orientation.

**n** — the principal quantum number. Controls the energy level and overall size of the orbital. Higher n means the electron is more likely to be found further from the nucleus.

**l** — the azimuthal quantum number. Controls the shape of the orbital. Must satisfy 0 <= l < n. At l=0 you get spherical orbitals, higher values introduce more complex geometries.

**m** — the magnetic quantum number. Controls the orientation of the orbital. Must satisfy -l <= m <= l. For orbitals with l > 0, different values of m produce the same shape rotated in different directions.

---

## Roadmap

- Density-based point coloring
- Probability current flow animation
- Instanced sphere rendering for a higher quality visual
- Photon absorption and emission system

---

## Background

This project was directly inspired by a video by Atom Simulator on YouTube by Kavan (Goated dude), which walks through the progression from the classical Bohr model all the way to the quantum mechanical picture. The goal here was to go beyond watching and actually implement the underlying mathematics from scratch. wish me luck!
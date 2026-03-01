# Understanding the Atom Simulator in C++

## Folder Structure and Scaffolding

**Day One — 1st March 2026**

The source folder is the main engine of this project. There are three core parts: the engine, the physics, and the renderer.

---

## The Engine

The engine is the main entrypoint to the system. It has two components — the window generator and the camera.

### Camera

The camera makes navigating the system easier through multiple viewpoints. It moves along the X, Y, and Z axes with polar coordinates added for compatibility with the simulation environment. Using trigonometrical functions, the radius is converted into viewpoints through a simple transformation function. The viewpoints react to mouse and keyboard input to zoom in and move along the Cartesian axis for proper showcase of the atoms.

### Engine

The engine has one singular purpose — it creates the window that is rendered when the program starts. It initializes an OpenGL instance with the feature and function flags necessary to run the program. It also lays down a set of instructions the compiler follows when rendering the window, and handles how to unload and clear the stack when the program closes.

### Header Files

Alongside the two components above are header files. These give proper declarations and tell the compiler where the dependent files are for each `.cpp` call.

---

## The Physics Directory

The physics directory is the heart of the project.

### Particle

Particles are initialized within a 3D space and assigned a role through future functions and classes. The `ParticleType` enum differentiates these particles, and the `getColor()` function assigns them a colour for rendering.

There are three integers — `energyLevel`, `orbitalShape`, and `orbitalRotation`. These are the quantum numbers n, l, and m. These integers are needed to calculate the state the atom occupies at any given point in time. When an electron absorbs a photon and jumps to a higher energy level, a timer starts counting down. When it reaches zero, the electron shifts back to a lower energy level and emits a photon.

### Atom

`Atom` is a container that groups particles together into a single atom. The atom has its own position in the 3D rendering space and its particles are rendered relative to it. This grouping allows the creation of multiple atoms in a scene and lets the program iterate over them cleanly without mixing up which particles belong to which atom.

### Photon

Photons are packets of energy that travel through the scene in a straight line. Their energy, wavelength, and frequency all describe the same physical property from different angles. These values matter because an electron can only absorb a photon if its energy exactly matches the gap between two energy levels. A photon with the wrong energy passes straight through.

---

## QMSolver — The Maths

This is the file that handles the physics and mathematics behind the project. It solves the Schrodinger equation and turns the result into actual 3D positions.

Using Schrodinger's equation, the wave function is split into two independent parts. The radial part tells us how far the electron is likely to be from the nucleus. The angular part tells us which direction we are looking at the atom from. Solving them separately is far simpler than tackling the full 3D problem at once, which is why polar coordinates are used instead of X, Y, and Z directly.

### Angular Wave Function

This computes the wave's strength at a given angle. It uses the associated Legendre polynomials, which are the angular equivalent of the Laguerre polynomials used in the radial calculation. Computing the spherical harmonics gives us the shape of the orbital — this is where the geometry of s, p, and d orbitals comes from.

### CDF Sampling

Once we have the probability distribution, it needs to be turned into actual points in space. Rather than hardcoding positions or taking a brute force approach, CDF (Cumulative Distribution Function) sampling works as follows:

1. Divide the range into a large number of bins
2. Compute the probability of each bin
3. Build the cumulative sum across all bins
4. Normalize the total to 1.0
5. Generate a random number between 0 and 1 and find where it lands on the cumulative list

The result is that high probability regions naturally produce more points and low probability regions produce fewer, without any wasted computation.

### Rendering in 3D

The returned `glm::vec3` represents one possible position of the electron. Calling this function 50,000 times produces the full orbital cloud — a point cloud that accurately reflects the real shape of the orbital at the given quantum state.
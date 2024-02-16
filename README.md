# Particle Simulator
A simulation project for Distributed Computing (STDISCM) course. This involves simulating particles in a 2D space with elastic collisions. User must be able to add particles and walls to the simulation. The simulation must be able to calculate the collisions between particles and walls and between particles themselves. The simulation must be able to run in parallel with threading.

# Packages used
- [glad](https://github.com/Dav1dde/glad)
- [sdl2](https://github.com/libsdl-org/SDL)
- [imgui](https://github.com/ocornut/imgui)

# How to run in Visual Studio
1. Clone the repository
2. Install the vcpkg dependencies through terminal (since the vcpkg_installed is already included in the repository, you can skip this or delete the vcpkg_installed and then do this step)
	1. `vcpkg install`
3. Make sure to generate CMake cache
4. Run the project

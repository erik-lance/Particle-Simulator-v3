#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include "Particle.h"

class SimulatorGUI {
public:
	void Init(SDL_Window* window, SDL_GLContext gl_context, SDL_Renderer* renderer, const char* glsl_version);
	void setParticles(std::vector<Particle>& particles);
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();
private:
	std::vector<Particle>* particles;
	int m_particle_id = 0;
	int m_particle_x = 0;
	int m_particle_y = 0;
	int m_particle_angle = 0;
	int m_particle_velocity = 0;

	int m_obstacle_id = 0;
};
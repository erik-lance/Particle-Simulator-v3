#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include "Objects/Particle.h"

class SimulatorGUI {
public:
	void Init(SDL_Window* window, SDL_GLContext gl_context, SDL_Renderer* renderer, const char* glsl_version);
	void setParticles(std::vector<Particle>& particles);
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();

	// GUI Functions
	void MainMenuGUI();
	void ParticlesBatchGUI();

	// Clamper (Wrapper for ImGui::InputInt)
	void InputClamp(int& num, int min, int max);
	void InputClampRelativeStart(int& num, int min, int max, int& relative_max);
	void InputClampRelativeEnd(int& num, int min, int max, int & relative_min);

private:
	// GUI Variables
	int menu_size_x = 250;
	int menu_size_y = 300;
	int menu_pos_x = 1300;
	int menu_pos_y = 10;

	int particle_batch_size_x = 250;
	int particle_batch_size_y = 500;
	int particle_batch_pos_x = 1300;
	int particle_batch_pos_y = menu_pos_y + menu_size_y + 10;

	// Main Inputs
	std::vector<Particle>* particles;
	int m_particle_id = 0;
	int m_particle_x = 0;
	int m_particle_y = 0;
	int m_particle_angle = 0;
	int m_particle_velocity = 0;

	int m_obstacle_id = 0;

	// Batch Inputs
	int m_batch_size = 0;

	int method_one_angle = 0;
	int method_one_velocity = 0;
	int method_one_start_x = 0;
	int method_one_start_y = 0;
	int method_one_end_x = 0;
	int method_one_end_y = 0;

	int method_two_start_x = 0;
	int method_two_start_y = 0;
	int method_two_velocity = 0;
	int method_two_start_angle = 0;
	int method_two_end_angle = 0;

	int method_three_start_x = 0;
	int method_three_start_y = 0;
	int method_three_angle = 0;
	int method_three_start_velocity = 0;
	int method_three_end_velocity = 0;
	
};
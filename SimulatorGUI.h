#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_impl_opengl3.h>
#include "Objects/ObjectManager.h"

class SimulatorGUI {
public:
	void Init(SDL_Window* window, SDL_Renderer* renderer, double* delta, int* fps_ctr, bool* running);
	void setManager(ObjectManager* manager) { m_object_manager = manager; }
	ObjectManager* getManager() { return m_object_manager; }
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();

	// GUI Functions
	void MainMenuGUI();
	void ParticlesBatchGUI();
	void ParticlesBatchMethodOneGUI();
	void ParticlesBatchMethodTwoGUI();
	void ParticlesBatchMethodThreeGUI();
	void PresetsAndMenuGUI();

	// Clamper (Wrapper for ImGui::InputInt)
	void InputClamp(int& num, int min, int max);
	void InputClampRelativeStart(int& num, int min, int max, int& relative_max);
	void InputClampRelativeEnd(int& num, int min, int max, int & relative_min);

private:
	bool* running = new bool(true);
	double* delta_time = new double(0);
	int* fps = new int(0);
	// GUI Variables
	int menu_size_x = 1260;
	int menu_size_y = 160;
	int menu_pos_x = 10;
	int menu_pos_y = 730;

	int particle_batch_size_x = 300;
	int particle_batch_size_y = 60;
	int particle_batch_pos_x = 1290;
	int particle_batch_pos_y = 10;

	int particle_batch_method_size_x[3] = { particle_batch_size_x, particle_batch_size_x, particle_batch_size_x };
	int particle_batch_method_size_y[3] = { 220, 195, 195 };
	int particle_batch_method_pos_x[3] = { particle_batch_pos_x, particle_batch_pos_x, particle_batch_pos_x };
	int particle_batch_method_pos_y[3] = { 
		particle_batch_pos_y + particle_batch_size_y + 10, 
		particle_batch_method_pos_y[0] + particle_batch_method_size_y[0] + 10, 
		particle_batch_method_pos_y[1] + particle_batch_method_size_y[1] + 10 
	};

	// Main Inputs
	ObjectManager* m_object_manager;
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
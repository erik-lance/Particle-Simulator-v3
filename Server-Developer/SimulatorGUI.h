#pragma once

#include <imgui.h>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_impl_opengl3.h>
#include "Objects/ObjectManager.h"
#include "Globals.h"

constexpr int MAX_VELOCITY = 1000;

class SimulatorGUI {
public:
	SimulatorGUI(SDL_Window* window, SDL_Renderer* renderer, ObjectManager* manager);
	~SimulatorGUI();
	ObjectManager* getManager() { return m_object_manager; }
	void NewFrame();
	virtual void Update();
	void Render();

	// GUI Functions
	void MainMenuGUI();
	void TitleGUI();
	void InfoGUI();
	void ParticlesGUI();
	void ObstaclesGUI();
	void ExplorerGUI();
	void ParticlesBatchGUI();
	void ParticlesBatchMethodOneGUI();
	void ParticlesBatchMethodTwoGUI();
	void ParticlesBatchMethodThreeGUI();
	void PresetsAndMenuGUI();

	// Clamper (Wrapper for ImGui::InputInt)
	void InputClamp(int& num, int min, int max);
	void InputClampRelativeStart(int& num, int min, int max, int& relative_max);
	void InputClampRelativeEnd(int& num, int min, int max, int & relative_min);
	void ResolveMethodOne();
	void ResolveMethodTwo();
	void ResolveMethodThree();

private:

	// GUI Variables
	float menu_size_x = 1260.0f;
	float menu_size_y = 160.0f;
	float menu_pos_x = 10.0f;
	float menu_pos_y = 730.0f;

	float particle_batch_size_x = 300.0f;
	float particle_batch_size_y = 180.0f;
	float particle_batch_pos_x = 1290.0f;
	float particle_batch_pos_y = 10.0f;

	float particle_batch_method_size_x[3] = { particle_batch_size_x, particle_batch_size_x, particle_batch_size_x };
	float particle_batch_method_size_y[3] = { 200.0f, 175.0f, 175.0f };
	float particle_batch_method_pos_x[3] = { particle_batch_pos_x, particle_batch_pos_x, particle_batch_pos_x };
	float particle_batch_method_pos_y[3] = {
		particle_batch_pos_y + particle_batch_size_y + 10.0f, 
		particle_batch_method_pos_y[0] + particle_batch_method_size_y[0] + 10.0f, 
		particle_batch_method_pos_y[1] + particle_batch_method_size_y[1] + 10.0f
	};

	float presets_size_x = particle_batch_size_x;
	float presets_size_y = 100.0f;

	float presets_pos_x = particle_batch_pos_x;
	float presets_pos_y = particle_batch_method_pos_y[2] + particle_batch_method_size_y[2] + 10.0f;

	// Main Inputs
	ObjectManager* m_object_manager;
	int m_particle_id = 0;
	int m_particle_x = 0;
	int m_particle_y = 0;
	int m_particle_angle = 0;
	int m_particle_velocity = 0;

	int m_obstacle_id = 0;
	int m_obstacle_start_x = 0;
	int m_obstacle_end_x = 0;
	int m_obstacle_start_y = 0;
	int m_obstacle_end_y = 0;

	int m_explorer_x = 0;
	int m_explorer_y = 0;

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
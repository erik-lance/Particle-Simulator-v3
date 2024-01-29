#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class SimulatorGUI {
public:
	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();
private:
	int m_particle_id = 0;
	int m_particle_x = 0;
	int m_particle_y = 0;
	int m_particle_angle = 0;
	int m_particle_velocity = 0;

	int m_obstacle_id = 0;
};
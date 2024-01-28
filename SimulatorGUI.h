#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class SimulatorGUI {
public:
	void Init(GLFWwindow* window, const char8_t* glsl_version);
	virtual void Update();
	void Render();
	void Shutdown();
};
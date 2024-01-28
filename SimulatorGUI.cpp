#include "SimulatorGUI.h"

void SimulatorGUI::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

void SimulatorGUI::Update()
{
}

void SimulatorGUI::Render()
{
}

void SimulatorGUI::Shutdown()
{
}

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

void SimulatorGUI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
}

void SimulatorGUI::Update()
{	
	// Render GUI
	ImGui::Begin("Simulator");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("Total Time: %.3f s", ImGui::GetTime());
	ImGui::End();
}

void SimulatorGUI::Render()
{
	// Render Dear ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SimulatorGUI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

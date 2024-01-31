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

void SimulatorGUI::setParticles(std::vector<Particle> particles) { this->particles = particles; }

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
	ImGui::Begin("GUI", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	// Set window position
	ImGui::SetWindowPos(ImVec2(1290, 10));

	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("Total Time: %.3f s", ImGui::GetTime());

	// Input Sections (Particle)
	ImGui::Separator();
	ImGui::Text("Particle");
	ImGui::InputInt("ID", &m_particle_id);
	ImGui::InputInt("X", &m_particle_x);
	ImGui::InputInt("Y", &m_particle_y);
	ImGui::InputInt("Angle", &m_particle_angle);
	ImGui::InputInt("Velocity", &m_particle_velocity);

	if (ImGui::Button("Add Particle")) {

	}

	// Input Sections (Obstacle)
	ImGui::Separator();
	ImGui::Text("Obstacle");
	ImGui::InputInt("ID", &m_obstacle_id);

	ImGui::End();


	// Simulator Window
	ImGui::Begin("Simulator", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	// Set window position
	ImGui::SetWindowPos(ImVec2(0, 0));

	// Set window size and positiong
	ImGui::SetWindowSize(ImVec2(1280, 720));

	// Draw grid
	for (int i = 0; i < 1280; i += 10) { ImGui::GetWindowDrawList()->AddLine(ImVec2(i, 0), ImVec2(i, 720), IM_COL32(255, 255, 255, 255)); }
	for (int i = 0; i < 720; i += 10) { ImGui::GetWindowDrawList()->AddLine(ImVec2(0, i), ImVec2(1280, i), IM_COL32(255, 255, 255, 255)); }

	// Set background color
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(1280, 720), IM_COL32(0, 0, 0, 255));

	// Draw particles here
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}

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

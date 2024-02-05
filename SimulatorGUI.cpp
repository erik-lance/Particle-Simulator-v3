#include "SimulatorGUI.h"
#include <iostream>

void SimulatorGUI::Init(SDL_Window* window, SDL_GLContext gl_context, SDL_Renderer* renderer, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

void SimulatorGUI::setParticles(std::vector<Particle>& particles) { this->particles = &particles; }

void SimulatorGUI::NewFrame()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
}

void SimulatorGUI::Update()
{	
	// Render GUI
	ImGui::Begin("GUI", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	// Set window position
	ImGui::SetWindowPos(ImVec2(1290, 10));

	// FPS and Delta Time and total time
	// FPS updates every 0.5 seconds
	static double prev_time = 0;
	static int fps = 0;
	double current_time = ImGui::GetTime();
	if (current_time - prev_time >= 0.5) {
		prev_time = current_time;
		fps = ImGui::GetIO().Framerate;
	}
	ImGui::Text("FPS: %d", fps);
	ImGui::Text("Delta Time: %.4f", ImGui::GetIO().DeltaTime);
	ImGui::Text("Total Time: %.4f", ImGui::GetTime());


	// Input Sections (Particle)
	ImGui::Separator();
	ImGui::Text("Particle");
	ImGui::Text("ID: %d", m_particle_id);
	ImGui::InputInt("X", &m_particle_x);
	ImGui::InputInt("Y", &m_particle_y);
	ImGui::InputInt("Angle", &m_particle_angle);
	ImGui::InputInt("Velocity", &m_particle_velocity);

	if (ImGui::Button("Add Particle")) {
		std:: cout << "Particle Added" << std::endl;
		Particle p(m_particle_id, m_particle_x, m_particle_y, m_particle_angle, m_particle_velocity);
		particles->push_back(p);

		// Increment particle id
		m_particle_id++;
	}

	// Input Sections (Obstacle)
	ImGui::Separator();
	ImGui::Text("Obstacle");
	ImGui::InputInt("ID", &m_obstacle_id);

	ImGui::End();


}

void SimulatorGUI::Render()
{
	// Render Dear ImGui
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void SimulatorGUI::Shutdown()
{
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

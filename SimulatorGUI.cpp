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
	// Main Menu
	MainMenuGUI();

	// Particles Batch
	ParticlesBatchGUI();
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

void SimulatorGUI::MainMenuGUI()
{
	// Render GUI
	ImGui::Begin("GUI", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	// Set window position
	ImGui::SetWindowPos(ImVec2(menu_pos_x, menu_pos_y));

	// Set window size
	ImGui::SetWindowSize(ImVec2(menu_size_x, menu_size_y));

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

	// Clamp x and y to 0-1280 and 1-720
	ImGui::InputInt("X", &m_particle_x);
	ImGui::InputInt("Y", &m_particle_y);
	if (m_particle_x < 0) m_particle_x = 0;
	if (m_particle_x > 1280) m_particle_x = 1280;
	if (m_particle_y < 0) m_particle_y = 0;
	if (m_particle_y > 720) m_particle_y = 720;

	// Clamp angle to 0-360
	ImGui::InputInt("Angle", &m_particle_angle);
	if (m_particle_angle < 0) m_particle_angle = 0;
	if (m_particle_angle > 360) m_particle_angle = 360;

	// Clamp velocity to 1-50
	ImGui::InputInt("Velocity", &m_particle_velocity);
	if (m_particle_velocity < 1) m_particle_velocity = 1;
	if (m_particle_velocity > 50) m_particle_velocity = 50;

	if (ImGui::Button("Add Particle")) {
		std::cout << "Particle Added" << std::endl;
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

void SimulatorGUI::ParticlesBatchGUI()
{
	ImGui::Begin("Particle Batch", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Set window position
	ImGui::SetWindowPos(ImVec2(particle_batch_pos_x, particle_batch_pos_y));

	// Set window size
	ImGui::SetWindowSize(ImVec2(particle_batch_size_x, particle_batch_size_y));

	// Clamp batch size to 1-1000
	ImGui::InputInt("Batch Size", &m_batch_size);
	if (m_batch_size < 1) m_batch_size = 1;
	if (m_batch_size > 1000) m_batch_size = 1000;

	// Method 1 (Provide a start and end point)
	// Particles are added with a uniform distance between given start and end points
	ImGui::Separator();
	ImGui::Text("Method 1");

	// Clamp x and y to 0-1280 and 1-720
	ImGui::InputInt("Start X", &method_one_start_x);
	ImGui::InputInt("Start Y", &method_one_start_y);
	if (method_one_start_x < 0) method_one_start_x = 0;
	if (method_one_start_x > 1280) method_one_start_x = 1280;
	if (method_one_start_y < 0) method_one_start_y = 0;
	if (method_one_start_y > 720) method_one_start_y = 720;

	ImGui::InputInt("End X", &method_one_end_x);
	ImGui::InputInt("End Y", &method_one_end_y);
	if (method_one_end_x < 0) method_one_end_x = 0;
	if (method_one_end_x > 1280) method_one_end_x = 1280;
	if (method_one_end_y < 0) method_one_end_y = 0;

	// Angle and Velocity constant for all particles
	ImGui::InputInt("Angle", &method_one_angle);
	if (method_one_angle < 0) method_one_angle = 0;
	if (method_one_angle > 360) method_one_angle = 360;

	ImGui::InputInt("Velocity", &method_one_velocity);
	if (method_one_velocity < 1) method_one_velocity = 1;
	if (method_one_velocity > 50) method_one_velocity = 50;

	if (ImGui::Button("Add Particle Batch (Method 1)")) {
		std::cout << "Particle Batch Added (Method 1)" << std::endl;
		for (int i = 0; i < m_batch_size; i++) {
			// Calculate the distance between start and end points
			double distance = sqrt(pow(method_one_end_x - method_one_start_x, 2) + pow(method_one_end_y - method_one_start_y, 2));
			double x = method_one_start_x + (method_one_end_x - method_one_start_x) * i / m_batch_size;
			double y = method_one_start_y + (method_one_end_y - method_one_start_y) * i / m_batch_size;
			Particle p(m_particle_id, x, y, method_one_angle, method_one_velocity);
			particles->push_back(p);

			// Increment particle id
			m_particle_id++;
		}
	}



	// Method 2 (Provide a start angle and end angle)
	// Particles are added with a uniform distance between given start and end angles

	// Method 3 (Provide a start velocity and end velocity)
	// Particles are added with a uniform distance between given start and end velocities

	ImGui::End();
}

void SimulatorGUI::InputClamp(const char* text, int& num, int min, int max)
{
	ImGui::InputInt(text, &num);
	if (num < min) num = min;
	if (num > max) num = max;
}
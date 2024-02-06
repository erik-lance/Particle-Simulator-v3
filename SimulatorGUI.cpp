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

	// Create a two column for statistics
	ImGui::BeginTable("Stats", 4);
	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	// Title
	ImGui::Text("Particle Simulator");
	ImGui::Text("STDISCM S11");
	ImGui::Spacing();

	ImGui::Text("Authors:");
	ImGui::Text("P1: Erik Tiongquico");
	ImGui::Text("P2: Clyla Rafanan");

	// Data
	ImGui::TableNextColumn();
	ImGui::Text("FPS: %d", fps);
	ImGui::Text("Delta Time: %.4f", ImGui::GetIO().DeltaTime);
	ImGui::Text("Total Time: %.4f", ImGui::GetTime());

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Number of Particles: %d", m_particle_id);
	ImGui::Text("Number of Obstacles: %d", m_obstacle_id);

	// Input Sections (Particle)
	ImGui::TableNextColumn();
	ImGui::Text("Particle");

	// Clamp x and y to 0-1280 and 1-720
	ImGui::InputInt("X", &m_particle_x);
	ImGui::InputInt("Y", &m_particle_y);
	InputClamp(m_particle_x, 0, 1280);
	InputClamp(m_particle_y, 0, 720);

	// Clamp angle to 0-360
	ImGui::InputInt("Angle", &m_particle_angle);
	InputClamp(m_particle_angle, 0, 360);

	// Clamp velocity to 1-50
	ImGui::InputInt("Velocity", &m_particle_velocity);
	InputClamp(m_particle_velocity, 1, 50);

	if (ImGui::Button("Add Particle")) {
		std::cout << "Particle Added" << std::endl;
		Particle p(m_particle_id, m_particle_x, m_particle_y, m_particle_angle, m_particle_velocity);
		particles->push_back(p);

		// Increment particle id
		m_particle_id++;
	}

	// Input Sections (Obstacle)
	ImGui::TableNextColumn();
	ImGui::Text("Obstacle");

	ImGui::EndTable();

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
	InputClamp(m_batch_size, 1, 1000);

	// Method 3 (Provide a start velocity and end velocity)
	// Particles are added with a uniform distance between given start and end velocities

	ImGui::End();

	ParticlesBatchMethodOneGUI();
	ParticlesBatchMethodTwoGUI();
	ParticlesBatchMethodThreeGUI();
}

void SimulatorGUI::ParticlesBatchMethodOneGUI()
{
	ImGui::Begin("Particle Batch (Method 1)", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Set Window
	ImGui::SetWindowPos(ImVec2(particle_batch_method_pos_x[0], particle_batch_method_pos_y[0]));
	ImGui::SetWindowSize(ImVec2(particle_batch_method_size_x[0], particle_batch_method_size_y[0]));

	// Method 1 (Provide a start and end point)
	// Particles are added with a uniform distance between given start and end points
	ImGui::Separator();
	ImGui::Text("Method 1");

	// Relatively Clamp x and y to 0-1280 and 1-720
	// So that the start and end points are within the window
	ImGui::InputInt("Start X", &method_one_start_x);
	InputClampRelativeStart(method_one_start_x, 0, 1280, method_one_end_x);

	ImGui::InputInt("End X", &method_one_end_x);
	InputClampRelativeEnd(method_one_end_x, 0, 1280, method_one_start_x);

	ImGui::InputInt("Start Y", &method_one_start_y);
	InputClampRelativeStart(method_one_start_y, 0, 720, method_one_end_y);

	ImGui::InputInt("End Y", &method_one_end_y);
	InputClampRelativeEnd(method_one_end_y, 0, 720, method_one_start_y);


	// Angle and Velocity constant for all particles
	ImGui::InputInt("Angle", &method_one_angle);
	InputClamp(method_one_angle, 0, 360);
	ImGui::InputInt("Velocity", &method_one_velocity);
	InputClamp(method_one_velocity, 1, 50);

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

	ImGui::End();
}

void SimulatorGUI::ParticlesBatchMethodTwoGUI()
{
	ImGui::Begin("Particle Batch (Method 2)", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Set Window
	ImGui::SetWindowPos(ImVec2(particle_batch_method_pos_x[1], particle_batch_method_pos_y[1]));
	ImGui::SetWindowSize(ImVec2(particle_batch_method_size_x[1], particle_batch_method_size_y[1]));

	// Method 2 (Provide a start angle and end angle)
	// Particles are added with a uniform distance between given start and end angles
	ImGui::Text("Method 2");

	// Relatively Clamp Angle Start and End to 0-360
	ImGui::InputInt("Start Angle", &method_two_start_angle);
	InputClampRelativeStart(method_two_start_angle, 0, 360, method_two_end_angle);

	ImGui::InputInt("End Angle", &method_two_end_angle);
	InputClampRelativeEnd(method_two_end_angle, 0, 360, method_two_start_angle);

	ImGui::InputInt("Start X", &method_two_start_x);
	InputClamp(method_two_start_x, 0, 1280);

	ImGui::InputInt("Start Y", &method_two_start_y);
	InputClamp(method_two_start_y, 0, 720);

	ImGui::InputInt("Velocity", &method_two_velocity);
	InputClamp(method_two_velocity, 1, 50);

	if (ImGui::Button("Add Particle Batch (Method 2)")) {
		std::cout << "Particle Batch Added (Method 2)" << std::endl;
		for (int i = 0; i < m_batch_size; i++) {
			double angle = method_two_start_angle + (method_two_end_angle - method_two_start_angle) * i / m_batch_size;
			Particle p(m_particle_id, method_two_start_x, method_two_start_y, angle, method_two_velocity);
			particles->push_back(p);

			// Increment particle id
			m_particle_id++;
		}
	}

	ImGui::End();
}

void SimulatorGUI::ParticlesBatchMethodThreeGUI()
{
	ImGui::Begin("Particle Batch (Method 3)", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Set Window
	ImGui::SetWindowPos(ImVec2(particle_batch_method_pos_x[2], particle_batch_method_pos_y[2]));
	ImGui::SetWindowSize(ImVec2(particle_batch_size_x, particle_batch_size_y));

	ImGui::End();
}

void SimulatorGUI::InputClamp(int& num, int min, int max) { if (num < min) num = min; if (num > max) num = max; }

void SimulatorGUI::InputClampRelativeStart(int& num, int min, int max, int& relative_max)
{
	if (num < min) num = min;
	if (num > relative_max && num <= max) relative_max = num;	// If start is greater than end, set end to start (Increment)
}

void SimulatorGUI::InputClampRelativeEnd(int& num, int min, int max, int& relative_min)
{
	if (num > max) num = max;
	if (num < relative_min && num >= min) relative_min = num;		// If end is less than start, set start to end (Decrement)
}
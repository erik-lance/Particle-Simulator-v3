#include "SimulatorGUI.h"
#include <iostream>

void SimulatorGUI::Init(SDL_Window* window, SDL_Renderer* renderer, double* delta, int* fps_ctr, bool* running)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Set timers
	delta_time = delta;
	fps = fps_ctr;

	// Set running
	this->running = running;
}

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

	// Presets and Menu
	PresetsAndMenuGUI();
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
	ImGui::Text("FPS: %d", *fps);
	ImGui::Text("Delta Time: %.4f", *delta_time);
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

	ImGui::Spacing();

	if (ImGui::Button("Add Particle")) {
		std::cout << "Particle Added" << std::endl;
		
		m_object_manager->addParticle(m_particle_x, m_particle_y, m_particle_angle, m_particle_velocity);

		// Increment particle id
		m_particle_id++;
	}

	// Input Sections (Obstacle)
	ImGui::TableNextColumn();
	ImGui::Text("Obstacle");

	if (ImGui::Button("Add Wall")) {
		std::cout << "Wall Added" << std::endl;
		Line line = Line();
		line.start.x = 512;
		line.start.y = 255;
		line.end.x = 1024;
		line.end.y = 512;

		// Calculate angle of the line in radians
		double dx = line.end.x - line.start.x;
		double dy = line.end.y - line.start.y;
		line.angle = atan2(dy, dx);

		// Normalize the angle
		line.angle = normalizeAngle(line.angle);

		std::cout << "Wall: " << line.start.x << " " << line.start.y << " " << line.end.x << " " << line.end.y << std::endl;
		std::cout << "Wall Angle: " << line.angle << std::endl;
		m_object_manager->addWall(line);

		// Increment obstacle id
		m_obstacle_id++;
	}

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
	InputClamp(m_batch_size, 1, 100000);

	// Separator
	ImGui::Separator();

	// Display error if any of the batch methods' start is greater than end or vice versa
	// and button to automatically fix the error
	if (method_one_start_x > method_one_end_x || method_one_start_y > method_one_end_y) {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: Method 1 Start is greater than End");
		if (ImGui::Button("Fix Error (Method 1)")) {
			ResolveMethodOne();
		}
	}

	if (method_two_start_angle > method_two_end_angle) {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: Method 2 Start is greater than End");
		if (ImGui::Button("Fix Error (Method 2)")) {
			ResolveMethodTwo();
		}
	}

	if (method_three_start_velocity > method_three_end_velocity) {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: Method 3 Start is greater than End");
		if (ImGui::Button("Fix Error (Method 3)")) {
			ResolveMethodThree();
		}
	}

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

	// Relatively Clamp x and y to 0-1280 and 1-720
	// So that the start and end points are within the window
	ImGui::InputInt("Start X", &method_one_start_x);
	ImGui::InputInt("End X", &method_one_end_x);

	InputClamp(method_one_start_x, 0, 1280);
	InputClamp(method_one_end_x, 0, 1280);

	ImGui::InputInt("Start Y", &method_one_start_y);
	ImGui::InputInt("End Y", &method_one_end_y);

	InputClamp(method_one_start_y, 0, 720);
	InputClamp(method_one_end_y, 0, 720);


	// Angle and Velocity constant for all particles
	ImGui::InputInt("Angle", &method_one_angle);
	InputClamp(method_one_angle, 0, 360);
	ImGui::InputInt("Velocity", &method_one_velocity);
	InputClamp(method_one_velocity, 1, 50);

	ImGui::Spacing();

	if (ImGui::Button("Add Particle Batch (Method 1)")) {
		ResolveMethodOne();
		std::cout << "Particle Batch Added (Method 1)" << std::endl;
		for (int i = 0; i < m_batch_size; i++) {
			// Calculate the distance between start and end points
			double distance = sqrt(pow(method_one_end_x - method_one_start_x, 2) + pow(method_one_end_y - method_one_start_y, 2));
			double x = method_one_start_x + (method_one_end_x - method_one_start_x) * i / m_batch_size;
			double y = method_one_start_y + (method_one_end_y - method_one_start_y) * i / m_batch_size;

			int pos_x = (int)x;
			int pos_y = (int)y;

			m_object_manager->addParticle(pos_x, pos_y, method_one_angle, method_one_velocity);

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

	// Relatively Clamp Angle Start and End to 0-360
	ImGui::InputInt("Start Angle", &method_two_start_angle);
	ImGui::InputInt("End Angle", &method_two_end_angle);

	InputClamp(method_two_start_angle, 0, 360);
	InputClamp(method_two_end_angle, 0, 360);

	ImGui::InputInt("Start X", &method_two_start_x);
	InputClamp(method_two_start_x, 0, 1280);

	ImGui::InputInt("Start Y", &method_two_start_y);
	InputClamp(method_two_start_y, 0, 720);

	ImGui::InputInt("Velocity", &method_two_velocity);
	InputClamp(method_two_velocity, 1, 50);

	ImGui::Spacing();

	if (ImGui::Button("Add Particle Batch (Method 2)")) {
		ResolveMethodTwo();
		std::cout << "Particle Batch Added (Method 2)" << std::endl;
		for (int i = 0; i < m_batch_size; i++) {
			double angle = method_two_start_angle + (method_two_end_angle - method_two_start_angle) * i / m_batch_size;
			m_object_manager->addParticle(method_two_start_x, method_two_start_y, angle, method_two_velocity);

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
	ImGui::SetWindowSize(ImVec2(particle_batch_method_size_x[2], particle_batch_method_size_y[2]));

	// Method 3 (Provide a start velocity and end velocity)
	// Particles are added with a uniform distance between given start and end velocities

	ImGui::InputInt("Start X", &method_three_start_x);
	InputClamp(method_three_start_x, 0, 1280);

	ImGui::InputInt("Start Y", &method_three_start_y);
	InputClamp(method_three_start_y, 0, 720);

	ImGui::InputInt("Angle", &method_three_angle);
	InputClamp(method_three_angle, 0, 360);

	ImGui::InputInt("Start Velocity", &method_three_start_velocity);
	ImGui::InputInt("End Velocity", &method_three_end_velocity);

	InputClamp(method_three_start_velocity, 1, 50);
	InputClamp(method_three_end_velocity, 1, 50);

	ImGui::Spacing();

	if (ImGui::Button("Add Particle Batch (Method 3)")) {
		ResolveMethodThree();
		std::cout << "Particle Batch Added (Method 3)" << std::endl;
		for (int i = 0; i < m_batch_size; i++) {
			double velocity = method_three_start_velocity + (method_three_end_velocity - method_three_start_velocity) * i / m_batch_size;
			m_object_manager->addParticle(method_three_start_x, method_three_start_y, method_three_angle, velocity);

			// Increment particle id
			m_particle_id++;
		}
	}

	ImGui::End();
}

void SimulatorGUI::PresetsAndMenuGUI()
{
	ImGui::Begin("Presets", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Set window
	ImGui::SetWindowPos(ImVec2(presets_pos_x, presets_pos_y));
	ImGui::SetWindowSize(ImVec2(presets_size_x, presets_size_y));

	// Presets
	if (ImGui::Button("Preset 1")) {
		std::cout << "Preset 1" << std::endl;

	}

	ImGui::SameLine();

	if (ImGui::Button("Preset 2")) {
		std::cout << "Preset 2" << std::endl;
	}

	ImGui::SameLine();

	if (ImGui::Button("Preset 3")) {
		std::cout << "Preset 3" << std::endl;
	}

	ImGui::SameLine();

	if (ImGui::Button("Preset 4")) {
		std::cout << "Preset 4" << std::endl;
	}

	// Exit Button float bottom right
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 50);
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);

	if (ImGui::Button("Exit")) {
		std::cout << "Exit" << std::endl;

		// Stop the program
		*running = false;
	}

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

void SimulatorGUI::ResolveMethodOne()
{
	if (method_one_start_x > method_one_end_x) method_one_start_x = method_one_end_x;
	if (method_one_start_y > method_one_end_y) method_one_start_y = method_one_end_y;
}

void SimulatorGUI::ResolveMethodTwo()
{
	if (method_two_start_angle > method_two_end_angle) method_two_start_angle = method_two_end_angle;
}

void SimulatorGUI::ResolveMethodThree()
{
	if (method_three_start_velocity > method_three_end_velocity) method_three_start_velocity = method_three_end_velocity;
}

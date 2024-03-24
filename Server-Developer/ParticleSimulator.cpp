// Particle Simulator.cpp : Defines the entry point for the application.
//

#define SDL_MAIN_HANDLED

#include "ParticleSimulator.h"
#include "SimulatorGUI.h"
#include "Globals.h"
#include <glad/glad.h>
#include <SDL.h>
#include <iostream>

// Window Dimensions
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

// Simulation Dimensions
const int SIM_WIDTH = 1280;
const int SIM_HEIGHT = 720;

// Explorer Dimensions
const int EXPLORER_WIDTH = 33;
const int EXPLORER_HEIGHT = 19;

using namespace std;

ObjectManager object_manager = ObjectManager(SIM_WIDTH, SIM_HEIGHT);

// Globals
bool isRunning = true;
double delta_time = 0;
int fps = 0;

bool drawGrid = false;

void draw_sim_borders(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 42, 74, 115, 255);
	SDL_RenderDrawLine(renderer, 1, 1, SIM_WIDTH, 1);
	SDL_RenderDrawLine(renderer, 0, 0, 0, SIM_HEIGHT);
	SDL_RenderDrawLine(renderer, SIM_WIDTH-1, 0, SIM_WIDTH-1, SIM_HEIGHT);
	SDL_RenderDrawLine(renderer, 0, SIM_HEIGHT-1, SIM_WIDTH, SIM_HEIGHT-1);
}

int main()
{
	// Setup SDL2
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		cerr << "Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Setup SDL Image
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		cerr << "Error: " << IMG_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	// Create window with graphics context
	SDL_Window* window = SDL_CreateWindow("Particle Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		cerr << "Window creation failed: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	// Create OpenGL context
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	if (gl_context == NULL) {
		cerr << "OpenGL context creation failed: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Initialized OpenGL loader (GLAD)
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		cerr << "Failed to initialize OpenGL context" << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// SDL Renderer Object with VSync
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		cerr << "Renderer creation failed: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Viewport for Simulation
	SDL_Rect sim_viewport = { 0, 0, SIM_WIDTH, SIM_HEIGHT };
	SDL_RenderSetViewport(renderer, &sim_viewport);

	SimulatorGUI gui = SimulatorGUI(window, renderer, &object_manager);

	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	// Calculate delta time
	Uint64 current_time = SDL_GetPerformanceCounter();
	Uint64 last_time = 0;

	// Set delta time of object manager
	object_manager.setDeltaTime(&delta_time);

	// Set Player
	object_manager.getPlayer()->loadSprite(renderer);

	while (isRunning) {
		// Calculate delta time
		last_time = current_time;
		current_time = SDL_GetPerformanceCounter();
		delta_time = (double)((current_time - last_time) / (double)SDL_GetPerformanceFrequency());

		// Process Input
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				SDL_GL_DeleteContext(gl_context);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0;
			}

		}

		// Clear the renderer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw_sim_borders(renderer);
		if (drawGrid) object_manager.drawGridLines(renderer);

		// Move the particles
		// object_manager.updateAndDrawParticles(delta_time, renderer);
		object_manager.updateAndDrawParticlesMultiThreaded(delta_time, renderer);

		// Update FPS every 0.5 seconds
		static double time = 0;
		time += delta_time * 1000;
		if (time > 500) {
			time = 0;
			fps = (int)(1 / (delta_time));
		}

		// Render ImGui
		gui.NewFrame();
		gui.Update();
		gui.Render();

		error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "OpenGL Error: " << error << std::endl;
		}

		SDL_RenderPresent(renderer);
	}

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

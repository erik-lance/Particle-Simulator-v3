// Particle Simulator.cpp : Defines the entry point for the application.
//

#define SDL_MAIN_HANDLED

#include "ParticleSimulator.h"
#include "SimulatorGUI.h"
#include "Particle.h"
#include <glad/glad.h>
#include <SDL.h>
#include <vector>

using namespace std;

int main()
{
	// Setup SDL2
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		cerr << "Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Create window with graphics context
	SDL_Window* window = SDL_CreateWindow("Particle Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

	// SDL Renderer Object
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		cerr << "Renderer creation failed: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}


	// Get screen size
	int screen_width, screen_height;
	SDL_GetWindowSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	// Particle Vector
	vector<Particle> particles = vector<Particle>();

	SimulatorGUI gui;
	gui.Init(window, gl_context, renderer, "#version 330");

	// Check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}

	

	while (1) {
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

		// Draw a line
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, 0, 0, 100, 100);

		// Draw particles
		for (int i = 0; i < particles.size(); i++) {
			particles[i].draw(renderer);
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

	gui.Shutdown();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

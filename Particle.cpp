#include "Particle.h"
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>

void Particle::updatePosition(double delta)
{
	// Convert the angle from degrees to radians
	double angle_rad = p_angle * M_PI / 180.0;

	// Update position based on velocity and angle using elapsed time
	pos_x += p_velocity * cos(angle_rad) * delta;
	pos_y += p_velocity * sin(angle_rad) * delta;
}

void Particle::handleScreenCollision(int screen_width, int screen_height)
{
	// Bounce off the walls
	if (pos_x < 0 || pos_x > screen_width) {
		// Reflect horizontally if particle hits left or right wall
		p_angle = 180 - p_angle;
	}
	if (pos_y < 0 || pos_y > screen_height) {
		// Reflect vertically if particle hits top or bottom wall
		p_angle = -p_angle;
	}
}

/**
 * @brief Draws the particle
 * @details This function draws the particle
 */
void Particle::draw(SDL_Renderer* renderer)
{
	// std::cout << "Drawing Particle at (" << pos_x << ", " << pos_y << ")" << std::endl;
	// Draws a white circle at the particle's position

	// Set color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Draw a circle
	for (int i = 0; i < 360; i++)
	{
		float x = pos_x + radius * cos(i);
		float y = pos_y + radius * sin(i);
		SDL_RenderDrawPoint(renderer, x, y);
	}
}

Particle::Particle(int id, int x, int y) {
	m_id = id;
	pos_x = x;
	pos_y = y;
	p_angle = 0;
	p_velocity = 0;
}

Particle::Particle(int id, int x, int y, int angle, int velocity)
{
	m_id = id;
	pos_x = x;
	pos_y = y;
	p_angle = angle;
	p_velocity = velocity;

	// Fix angle to counter-clockwise
	p_angle = 360 - p_angle;
}

Particle::~Particle()
{
	// TODO: Implement destructor
}

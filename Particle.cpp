#include "Particle.h"
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>

void Particle::updatePosition(double delta)
{
	// Update position based on velocity and angle using elapsed time
	pos_x += p_velocity * cos(p_angle) * delta;
	pos_y += p_velocity * sin(p_angle) * delta;
}

void Particle::handleScreenCollision(int screen_width, int screen_height)
{
	// Bounce off the walls
	// Reflect vertically if particle hits left or right wall
	if (pos_x < 0 || pos_x > screen_width) { p_angle = -p_angle; }
	// Reflect horizontally if particle hits top or bottom wall
	if (pos_y < 0 || pos_y > screen_height) { p_angle = M_PI - p_angle; }
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

	// Draw a point
	SDL_RenderDrawPoint(renderer, pos_x, pos_y);
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
}

Particle::~Particle()
{
	// TODO: Implement destructor
}

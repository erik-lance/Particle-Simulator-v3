#include "Particle.h"
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>

void Particle::updatePosition(double delta)
{
	// Convert the angle from degrees to radians
	double angle_rad = p_angle * M_PI / 180.0;

	// Update position based on velocity and angle using elapsed time
	position.x += p_velocity * cos(angle_rad) * delta;
	position.y += p_velocity * sin(angle_rad) * delta;
}

void Particle::handleScreenCollision()
{
	// Bounce off the walls
	if (position.x < 0 || position.x > screen_width) {
		// Reflect horizontally if particle hits left or right wall
		p_angle = 180 - p_angle;
	}
	if (position.y < 0 || position.y > screen_height) {
		// Reflect vertically if particle hits top or bottom wall
		p_angle = -p_angle;
	}
}

/**
 * Handles the particles collision given a line
 * @param x1 The x coordinate of the first point of the line
 * @param y1 The y coordinate of the first point of the line
 * @param x2 The x coordinate of the second point of the line
 * @param y2 The y coordinate of the second point of the line
 * @return True if there is a collision, false otherwise
 */
bool Particle::handleLineCollision(int x1, int y1, int x2, int y2)
{
	// Calculate the distance between the particle and the line
	// Formula: |(x2 - x1)(y1 - y) - (x1 - x)(y2 - y1)| / sqrt((x2 - x1)^2 + (y2 - y1)^2)
	double distance = abs((x2 - x1) * (y1 - position.y) - (x1 - position.x) * (y2 - y1)) / sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	// If the distance is less than the radius of the particle, then there is a collision
	if (distance < radius)
	{
		// Reflect the particle's angle
		p_angle = 180 - p_angle;
		return true;
	}
	else
	{
		return false;
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
		float x = position.x + radius * cos(i);
		float y = position.y + radius * sin(i);
		SDL_RenderDrawPoint(renderer, x, y);
	}
}

Particle::Particle(int id, int x, int y) {
	m_id = id;
	position.x = x;
	position.y = y;
	old_position.x = x;
	old_position.y = y;
	p_angle = 0;
	p_velocity = 0;
}

Particle::Particle(int id, int x, int y, int angle, int velocity)
{
	m_id = id;
	position.x = x;
	position.y = y;
	old_position.x = x;
	old_position.y = y;
	p_angle = angle;
	p_velocity = velocity;

	// Fix angle to counter-clockwise
	p_angle = 360 - p_angle;

	// Fix velocity to pixels per second
	p_velocity = (double)velocity / 10;
}

Particle::Particle()
{
	m_id = -1;
	position.x = 0;
	position.y = 0;
	old_position.x = 0;
	old_position.y = 0;
	p_angle = 0;
	p_velocity = 0;
}

Particle::~Particle()
{
	// TODO: Implement destructor
}
#include "Particle.h"
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>

void Particle::updatePosition(double delta)
{
	// Convert the angle from degrees to radians
	double angle_rad = p_angle * M_PI / 180.0;

	// Save the old position
	old_position.x = position.x;
	old_position.y = position.y;

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

		// Clamp the particle's position to the screen
		if (position.x < 0) position.x = 0;
		if (position.x > screen_width) position.x = screen_width;
	}
	if (position.y < 0 || position.y > screen_height) {
		// Reflect vertically if particle hits top or bottom wall
		p_angle = -p_angle;

		// Clamp the particle's position to the screen
		if (position.y < 0) position.y = 0;
		if (position.y > screen_height) position.y = screen_height;
	}
}

/**
 * Handles the particles collision given a line by checking if
 * there is an intersection between the particle and the line
 * @param line The line to check for collision
 * @return True if there is a collision, false otherwise
 */
bool Particle::handleLineCollision(Line line)
{

	// Check if particle's old and new position intersects with the line
	bool collided = false;

	Line particle_line = {old_position, position};

	Position intersection = lineIntersection(line, particle_line);

	if (intersection.x != 0 && intersection.y != 0)
	{
		std::cout << "Intersection at (" << intersection.x << ", " << intersection.y << ")" << std::endl;
		collided = true;
	}

	// Distance of new position to intersection point
	int distance = sqrt(pow(position.x - intersection.x, 2) + pow(position.y - intersection.y, 2));

	// Update the particle's position to the intersection point and adjust the angle
	if (collided)
	{
		std::cout << "Collision detected" << std::endl;
		std::cout << "Position: (" << position.x << ", " << position.y << ")" << std::endl;
		std::cout << "Angle: " << p_angle << std::endl;
		// Update the particle's position to the intersection point
		position.x = intersection.x;
		position.y = intersection.y;

		// Reflect the particle's angle
		// p_angle = reflectAngle(p_angle);
		p_angle = 180 - p_angle;

		// And then move the position by the distance
		position.x += distance * cos(p_angle * M_PI / 180);
		position.y += distance * sin(p_angle * M_PI / 180);

		std::cout << "New Position: (" << position.x << ", " << position.y << ")" << std::endl;
		std::cout << "New Angle: " << p_angle << std::endl;

		return true;
	}

	return false;
}

/**
 * @brief Draws the particle
 * @details This function draws the particle
 */
void Particle::draw(SDL_Renderer* renderer) const
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
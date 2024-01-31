#include "Particle.h"
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>

int Particle::getID() { return m_id; }
int Particle::getX() { return pos_x; }
int Particle::getY() { return pos_y; }
int Particle::getAngle() { return p_angle; }
int Particle::getVelocity() { return p_velocity; }

void Particle::setX(int x) { pos_x = x; }
void Particle::setY(int y) { pos_y = y; }
void Particle::setAngle(int angle) { p_angle = angle; }
void Particle::setVelocity(int velocity) { p_velocity = velocity; }

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

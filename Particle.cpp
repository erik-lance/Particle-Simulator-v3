#include "Particle.h"

int Particle::getID() { return m_id; }
int Particle::getX() { return pos_x; }
int Particle::getY() { return pos_y; }
int Particle::getAngle() { return p_angle; }
int Particle::getVelocity() { return p_velocity; }

void Particle::setX(int x) { pos_x = x; }
void Particle::setY(int y) { pos_y = y; }
void Particle::setAngle(int angle) { p_angle = angle; }
void Particle::setVelocity(int velocity) { p_velocity = velocity; }

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

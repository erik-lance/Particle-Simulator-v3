#pragma once

#include <SDL.h>

/**
* @brief Particle class
* @details This class is used to create a particle object
* @param m_id The particle's ID
* @param pos_x The particle's x position
* @param pos_y The particle's y position
* @param p_angle The particle's angle
* @param p_velocity The particle's velocity
* @return Particle object
*/
class Particle {
public:
	Particle(int id, int x, int y);
	Particle(int id, int x, int y, int angle, int velocity);
	~Particle();

	int getID() const { return m_id; }
	int getX() const { return pos_x; }
	int getY() const { return pos_y; }
	int getAngle() const { return p_angle; }
	int getVelocity() const { return p_velocity; }

	void setX(int x) { pos_x = x; }
	void setY(int y) { pos_y = y; }
	void setAngle(int angle) { p_angle = angle; }
	void setVelocity(int velocity) { p_velocity = velocity; }

	void updatePosition(double delta);
	void handleScreenCollision(int screen_width, int screen_height);

	void draw(SDL_Renderer* renderer);

private:
	int m_id;
	int pos_x;
	int pos_y;

	int p_angle;
	int p_velocity;
};

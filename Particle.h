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

	int getID();
	int getX();
	int getY();
	int getAngle();
	int getVelocity();

	void setX(int x);
	void setY(int y);
	void setAngle(int angle);
	void setVelocity(int velocity);

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

#pragma once

#include <SDL.h>
#include "../Structures.h"

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
	Particle();
	~Particle();

	int getID() const { return m_id; }
	Position getPosition() const { return position; }
	Position getOldPosition() const { return old_position; }
	int getAngle() const { return p_angle; }
	int getVelocity() const { return p_velocity; }

	void setID(int id) { m_id = id; }
	void setPosition(Position pos) { position = pos; }
	void setAngle(int angle) { p_angle = angle; }
	void setVelocity(int velocity) { p_velocity = velocity; }
	void setScreenSize(int width, int height) { screen_width = width; screen_height = height; }

	void updatePosition(double delta);
	void handleScreenCollision();
	bool handleLineCollision(Line line);

	void draw(SDL_Renderer* renderer) const;

private:
	int m_id;
	Position position;
	Position old_position;

	int screen_width = 1280;
	int screen_height = 720;

	double p_angle;
	double p_velocity;
	int radius = 3;
};

#pragma once
#include "Particle.h"
#include "Wall.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void addParticle(int x, int y, int angle, int velocity);
	void addWall(int x1, int y1, int x2, int y2);

	void updateParticles(double delta);
	void updateAndDrawParticles(double delta, SDL_Renderer* renderer);
	void drawWalls(SDL_Renderer* renderer);

	int getParticleCount() { return current_max_particles; }
	int getWallCount() { return current_max_walls; }

private:
	int initial_capacity = 1024;
	int particle_capacity = initial_capacity;
	int wall_capacity = initial_capacity;
	int current_max_particles = 0;
	int current_max_walls = 0;
	Particle* particles = new Particle[particle_capacity];
	Wall* walls = new Wall[wall_capacity];

};

ObjectManager::ObjectManager()
{

}

/**
 * Safely deallocates memory for the particles and walls
 */
ObjectManager::~ObjectManager()
{
	delete[] particles;
	delete[] walls;
}
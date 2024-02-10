#pragma once
#include "../CollisionManager.h"
#include "Particle.h"
#include "Wall.h"
#include "DebugCircle.h"

class ObjectManager
{
public:
	ObjectManager();
	ObjectManager(int width, int height);
	~ObjectManager();

	void addParticle(int x, int y, int angle, int velocity);
	void addWall(Line line);

	void updateParticles(double delta);
	void updateAndDrawParticles(double delta, SDL_Renderer* renderer);
	void drawWalls(SDL_Renderer* renderer);

	int getParticleCount() { return current_max_particles; }
	int getWallCount() { return current_max_walls; }
	CollisionManager getCollisionManager() { return *collision_manager; }

	// Debug
	void drawDebugCircles(SDL_Renderer* renderer);
	void addDebugCircle(Position pos, int r);

private:
	int screen_width, screen_height;
	CollisionManager* collision_manager;

	int initial_capacity = 1024;
	int particle_capacity = initial_capacity;
	int wall_capacity = initial_capacity;
	int current_max_particles = 1;
	int current_max_walls = 1;
	Particle* particles = new Particle[particle_capacity];
	Wall* walls = new Wall[wall_capacity];
	
	DebugCircle debug_circles[10];
};

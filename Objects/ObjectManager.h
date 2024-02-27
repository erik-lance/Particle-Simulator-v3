#pragma once
#include <thread>
#include "../CollisionManager.h"
#include "Particle.h"
#include "Wall.h"
#include "Player.h"
#include "DebugCircle.h"
#include "../Globals.h"

constexpr int THREAD_COUNT = 4;

struct ParticleThreadData
{
	int capacity = 256;
	int* indices = new int[capacity];
	int count = 1;
	int jobs = 0;
};

class ObjectManager
{
public:
	ObjectManager();
	ObjectManager(int width, int height);
	~ObjectManager();

	void setDeltaTime(double* delta) { cur_delta = delta; }

	void addParticle(int x, int y, double angle, double velocity);
	void distributeParticleToThread(int index);
	void addWall(Line line);

	void setupThreads();
	void threadLoop(int index);
	void updateParticles(double delta);
	void updateAndDrawParticles(double delta, SDL_Renderer* renderer);
	void updateAndDrawParticlesMultiThreaded(double delta, SDL_Renderer* renderer);
	void updateAndDrawParticlesIndices(int* indices, int count);
	void drawWalls(SDL_Renderer* renderer);

	int getParticleCount() const { return current_max_particles; }
	int getWallCount() const { return current_max_walls; }
	CollisionManager getCollisionManager() { return *collision_manager; }

	Player* getPlayer() { return &player; }

	// Debug
	void drawGridLines(SDL_Renderer* renderer);
	void drawDebugCircles(SDL_Renderer* renderer);
	void addDebugCircle(Position pos, int r);

private:
	int screen_width, screen_height;
	CollisionManager* collision_manager;
	
	// Object Threads (16)
	std::thread object_threads[THREAD_COUNT];
	ParticleThreadData thread_data[THREAD_COUNT];
	double* cur_delta = new double(0);
	bool running = true;

	int initial_capacity = 1024;
	int particle_capacity = initial_capacity;
	int wall_capacity = initial_capacity;
	int current_max_particles = 1;
	int current_max_walls = 1;
	Particle* particles = new Particle[particle_capacity];
	Wall* walls = new Wall[wall_capacity];

	Player player = Player(Position(640, 360));
	
	DebugCircle debug_circles[10];
};

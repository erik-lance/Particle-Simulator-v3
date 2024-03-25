#pragma once
#include <thread>
#include "Particle.h"
#include "Player.h"
#include "../Globals.h"
#include <vector>
#include <string>

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
	void drawGridLines(SDL_Renderer* renderer);

	void setupThreads();
	void threadLoop(int index);
	void updateParticles(double delta);
	void updateAndDrawParticles(double delta, SDL_Renderer* renderer);
	void updateAndDrawParticlesMultiThreaded(double delta, SDL_Renderer* renderer);
	void updateAndDrawParticlesIndices(int* indices, int count);

	int getParticleCount() const { return current_max_particles; }

	std::vector<Player> getPlayers() const { return players; }
	Player getPlayer(int index) const { return players[index]; }
	void addPlayer(Player player) { players.push_back(player); }

private:
	int screen_width, screen_height;
	
	// Object Threads (16)
	std::thread object_threads[THREAD_COUNT];
	ParticleThreadData thread_data[THREAD_COUNT];
	double* cur_delta = new double(0);
	bool running = true;

	int initial_capacity = 1024;
	int particle_capacity = initial_capacity;
	int current_max_particles = 1;
	Particle* particles = new Particle[particle_capacity];

	std::vector<Player> players = std::vector<Player>();
};

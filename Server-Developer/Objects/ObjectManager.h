#pragma once
#include "Particle.h"
#include "Player.h"
#include "../Globals.h"
#include "../Structures.h"
#include <vector>
#include <chrono>
#include <mutex>

class ObjectManager
{
public:
	ObjectManager();
	ObjectManager(int width, int height);
	~ObjectManager();

	void setDeltaTime(double* delta) { cur_delta = delta; }

	void addParticle(int x, int y, double angle, double velocity);
	void drawGridLines();

	void updateParticles(double delta);
	void updateAndDrawParticles(SDL_Renderer* renderer, double delta);
	void updateAndDrawParticlesIndices(int* indices, int count);

	int getParticleCount() const { return current_max_particles; }

	Player* getPlayers() const { return players; }
	Player getPlayer(int index) const { return players[index]; }
	void generatePlayer(std::string UUID, Position pos);
	void readyPlayers(SDL_Renderer* renderer);
	void updatePlayerMovement(int index, Position pos, Position dir) { players[index].setPosition(pos); players[index].updateDirection(dir); }

	void logParticleRecord(std::string command);
	std::vector<ParticleHistoryRecord> getParticleHistory() const { return particle_history; }

private:
	int screen_width, screen_height;
	SDL_Renderer* renderer;
	std::mutex mtx;
	
	double* cur_delta = new double(0);
	bool running = true;

	int initial_capacity = 1024;
	int particle_capacity = initial_capacity;
	int current_max_particles = 1;
	Particle* particles = new Particle[particle_capacity];

	std::vector<Player> uninitialized_players = std::vector<Player>();
	int num_players = 0;
	Player* players = new Player[4];

	// Particle History
	std::vector<ParticleHistoryRecord> particle_history = std::vector<ParticleHistoryRecord>();
	std::chrono::time_point<std::chrono::system_clock> start_time;
};

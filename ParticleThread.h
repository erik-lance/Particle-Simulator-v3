#pragma once

#include "Particle.h"
#include <vector>
#include <thread>

class ParticleThread {
public:
	ParticleThread();
	~ParticleThread();

	void start();
	void join();

	void addParticle(Particle p);
	void removeParticle(int id);

private:
	std::vector<Particle> particles;

};

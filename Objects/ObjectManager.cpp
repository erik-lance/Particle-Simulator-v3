#include "ObjectManager.h"

/**
 * Adds particle to the particle array. If the array is full, it will double the capacity
 * @param x The particle's x position
 * @param y The particle's y position
 * @param angle The particle's angle
 * @param velocity The particle's velocity
 */
void ObjectManager::addParticle(int x, int y, int angle, int velocity)
{
    // If the array is full, double the capacity
    if (current_max_particles >= particle_capacity)
    {
        particle_capacity *= 2;
        Particle* new_particles = new Particle[particle_capacity];
        for (int i = 0; i < current_max_particles; i++)
        {
            new_particles[i] = particles[i];
        }
        delete[] particles;
        particles = new_particles;
    }

    // Add the particle to the array
    particles[current_max_particles] = Particle(current_max_particles, x, y, angle, velocity);
    current_max_particles++; // Increment the counter after adding the particle
}

/**
 * Adds wall to the wall array. If the array is full, it will double the capacity
 * @param x1 The wall's first x position
 * @param y1 The wall's first y position
 * @param x2 The wall's second x position
 * @param y2 The wall's second y 
 */
void ObjectManager::addWall(int x1, int y1, int x2, int y2)
{
	
}

void ObjectManager::updateParticles(double delta)
{

}

void ObjectManager::updateAndDrawParticles(double delta, SDL_Renderer* renderer)
{
}


void ObjectManager::drawWalls(SDL_Renderer* renderer)
{

}

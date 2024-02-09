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

    // Set the screen size for the particle
    particles[current_max_particles].setScreenSize(screen_width, screen_height);

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

/**
 * Updates the particle positions
 * @param delta The time elapsed since the last update
 * */
void ObjectManager::updateParticles(double delta)
{
    // Temporary non multi-threaded update
    for (int i = 0; i < current_max_particles; i++)
    {
		particles[i].updatePosition(delta);
	}
}

/**
 * Updates the particle positions and draws them
 * @param delta The time elapsed since the last update
 * @param renderer The SDL renderer to draw the particles
 */
void ObjectManager::updateAndDrawParticles(double delta, SDL_Renderer* renderer)
{
    // Temporary non multi-threaded update
    for (int i = 0; i < current_max_particles; i++)
    {
		particles[i].updatePosition(delta);
		particles[i].handleScreenCollision();
		particles[i].draw(renderer);
	}
}


void ObjectManager::drawWalls(SDL_Renderer* renderer)
{

}

ObjectManager::ObjectManager() { screen_width = 1280; screen_height = 720; }

ObjectManager::ObjectManager(int width, int height)
{
    screen_width = width;
	screen_height = height;
}

/**
 * Safely deallocates memory for the particles and walls
 */
ObjectManager::~ObjectManager()
{
    delete[] particles;
    delete[] walls;
}
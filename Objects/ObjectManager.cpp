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
    particles[current_max_particles-1] = Particle(current_max_particles, x, y, angle, velocity);

    // Set the screen size for the particle
    particles[current_max_particles-1].setScreenSize(screen_width, screen_height);

    current_max_particles++; // Increment the counter after adding the particle
}

/**
 * Adds wall to the wall array. If the array is full, it will double the capacity
 * @param line The line to add to the wall array
 */
void ObjectManager::addWall(Line line)
{
	// If the array is full, double the capacity
    if (current_max_walls >= wall_capacity)
    {
		wall_capacity *= 2;
		Wall* new_walls = new Wall[wall_capacity];
        for (int i = 0; i < current_max_walls; i++)
        {
			new_walls[i] = walls[i];
		}
		delete[] walls;
		walls = new_walls;
	}

	// Add the wall to the array
	walls[current_max_walls-1] = Wall(line);
    current_max_walls++; // Increment the counter after adding the wall

    // Add to the collision manager
    collision_manager->addWall(walls[current_max_walls-2]);
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

ObjectManager::ObjectManager() { screen_width = 1280; screen_height = 720; collision_manager = new CollisionManager(screen_width, screen_height); }

ObjectManager::ObjectManager(int width, int height)
{
    screen_width = width;
	screen_height = height;
    collision_manager = new CollisionManager(width, height);
}

/**
 * Safely deallocates memory for the particles and walls
 */
ObjectManager::~ObjectManager()
{
    delete[] particles;
    delete[] walls;
    delete &collision_manager;
}
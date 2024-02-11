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

void ObjectManager::setupThreads()
{
    // Initialize thread particle capacity
    // Assuming each thread can initially handle up to 256 particles
    for (int i = 0; i < 16; ++i) { thread_particle_capacity[i] = 256; }

    // Initialize particle indices array
    // Assuming each thread starts with an empty array
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < thread_particle_capacity[i]; ++j) {
            particle_indices[i][j] = -1; // -1 indicates an empty slot
        }
    }
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
    // Temporary non multi-threaded update and draw
    for (int i = 0; i < current_max_particles; i++)
	{
        particles[i].updatePosition(delta);
        collision_manager->checkParticleCollisionsInCells(&particles[i]);
        particles[i].handleScreenCollision();
        particles[i].draw(renderer);
    }
	
    // Draw debug circles
    // drawDebugCircles(renderer);
}

/**
 * Updates and draws the particles in the given range
 * @param delta The time elapsed since the last update
 * @param renderer The SDL renderer to draw the particles
 * @param start The start index of the range
 * @param end The end index of the range

 */
void ObjectManager::updateAndDrawParticlesRange(double delta, SDL_Renderer* renderer, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		particles[i].updatePosition(delta);
        collision_manager->checkParticleCollisionsInCells(&particles[i]);
        particles[i].handleScreenCollision();
		particles[i].draw(renderer);
	}
}

/**
 * Updates and draws the particles in the given indices array
 * @param delta The time elapsed since the last update
 * @param renderer The SDL renderer to draw the particles
 * @param indices The array of indices to update and draw
 * @param count The number of indices in the array
 */
void ObjectManager::updateAndDrawParticlesIndices(double delta, SDL_Renderer* renderer, int* indices, int count)
{
    // Iterate through the indices array and update and draw the particles
    for (int i = 0; i < count; i++)
	{
		particles[indices[i]].updatePosition(delta);
		collision_manager->checkParticleCollisionsInCells(&particles[indices[i]]);
		particles[indices[i]].handleScreenCollision();
		particles[indices[i]].draw(renderer);
	}
}


void ObjectManager::drawWalls(SDL_Renderer* renderer)
{
    for (int i = 0; i < current_max_walls; i++)
    {
		walls[i].draw(renderer);
	}
}

/**
 * Draws the grid lines based on the collision manager's grid
 * width and height and column and row count
 * @param renderer The SDL renderer to draw the grid lines
 */
void ObjectManager::drawGridLines(SDL_Renderer* renderer)
{
    int cell_width = collision_manager->getGridCellWidth();
    int cell_height = collision_manager->getGridCellHeight();
    int column_count = collision_manager->getGridColumns();
    int row_count = collision_manager->getGridRows();

    // Set the color to gray
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

	// Draw vertical lines
    for (int i = 0; i < column_count; i++)
    {
		SDL_RenderDrawLine(renderer, i * cell_width, 0, i * cell_width, screen_height);
	}

	// Draw horizontal lines
    for (int i = 0; i < row_count; i++)
    {
		SDL_RenderDrawLine(renderer, 0, i * cell_height, screen_width, i * cell_height);
	}
}

void ObjectManager::drawDebugCircles(SDL_Renderer* renderer)
{
    for (int i = 0; i < 10; i++) { debug_circles[i].draw(renderer); }
}

void ObjectManager::addDebugCircle(Position pos, int r)
{
    DebugCircle new_circle = DebugCircle(pos, r);
    for (int i = 0; i < 10; i++)
    {
        Position slot = debug_circles[i].getPosition();
        if (slot.x == -1 && slot.y == -1)
        {
			debug_circles[i] = new_circle;
			break;
		}
	}
}

ObjectManager::ObjectManager() { screen_width = 1280; screen_height = 720; collision_manager = new CollisionManager(screen_width, screen_height); setupThreads(); }

ObjectManager::ObjectManager(int width, int height)
{
    screen_width = width;
	screen_height = height;
    collision_manager = new CollisionManager(width, height);

    setupThreads();
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
#include "ObjectManager.h"

/**
 * Adds particle to the particle array. If the array is full, it will double the capacity
 * @param x The particle's x position
 * @param y The particle's y position
 * @param angle The particle's angle
 * @param velocity The particle's velocity
 */
void ObjectManager::addParticle(int x, int y, double angle, double velocity)
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
 * @param renderer The SDL renderer to draw the particles
 * @param delta The time elapsed since the last update
 */
void ObjectManager::updateAndDrawParticles(SDL_Renderer* renderer, double delta)
{
    for (int i = 0; i < current_max_particles; i++)
    {
        particles[i].updatePosition(delta);
		particles[i].handleScreenCollision();
        particles[i].draw(renderer);
    }

    // Draw player objects
    readyPlayers(renderer);

    for (int i = 0; i < num_players; i++)
    {
        players[i].move(delta);
        players[i].draw(renderer);
    }
}
/**
 * Updates and draws the particles in the given indices array
 * @param indices The array of indices to update and draw
 * @param count The number of indices in the array
 */
void ObjectManager::updateAndDrawParticlesIndices(int* indices, int count)
{
    // Iterate through the indices array and update and draw the particles
    for (int i = 0; i < count-1; i++)
	{
        particles[indices[i]].updatePosition(*cur_delta);
		particles[indices[i]].handleScreenCollision();
	}
}

/**
 * Generates a player object and adds it to the uninitialized players vector
 */
void ObjectManager::generatePlayer(std::string UUID, Position pos)
{
    std::cout << "Spawning " << UUID << " at position (" << pos.x << ", " << pos.y << ")\n" << std::endl;
    Player player = Player(UUID, pos);

    mtx.lock();
	uninitialized_players.push_back(player);
    mtx.unlock();
}

void ObjectManager::readyPlayers(SDL_Renderer* renderer)
{
    if (uninitialized_players.size() > 0)
    {
        // Get last character of UUID
        std::string last_char = uninitialized_players[0].getUUID().substr(uninitialized_players[0].getUUID().size() - 1);
        int num = std::stoi(last_char) % 4;

        std::cout << "New player detected, adding to player list" << std::endl;
        mtx.lock();

        players[num_players] = uninitialized_players[0];
		uninitialized_players.erase(uninitialized_players.begin());

        // Set Sprite Number of Player
        players[num_players].loadSpriteFromNumber(renderer, num);
        num_players++;
        mtx.unlock();

        std::cout << "Player added to player list (Player Size = " << num_players << ")" << std::endl;
	}
}

void ObjectManager::logParticleRecord(std::string command)
{
	// Log the particle record
	ParticleHistoryRecord record;
	record.ticks = 0;
	record.command = command;

    // If no logs yet, ticks = 0 and start the timer
    if (particle_history.size() == 0)
    {
		start_time = std::chrono::system_clock::now();
	}
    else
    {
		// Calculate the ticks since the last log
		auto end_time = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end_time - start_time;
		record.ticks = elapsed_seconds.count();

        // Reset the timer to count ticks since the last log
		start_time = std::chrono::system_clock::now();
	}

    // Add the record to the history
    particle_history.push_back(record);
}

std::vector<std::string> ObjectManager::getPreviousPlayers(std::string UUID)
{
    std::cout << "Getting previous players (" << num_players << ")\n" << std::endl;
    // Generate "<c>" for each player except the last one
    if (num_players > 0)
    {
		std::vector<std::string> previous_players;
        for (int i = 0; i < num_players; i++)
        {
            if (players[i].getUUID() == UUID) continue;
            Position pos = players[i].getPosition();
            std::string connection = "<c>" + players[i].getUUID() + ":" + std::to_string(pos.x) + "," + std::to_string(pos.y) + "</c>";
			previous_players.push_back(connection);
		}
		return previous_players;
	}
    return std::vector<std::string>();
}

/**
 * Draws the grid lines based on the collision manager's grid
 * width and height and column and row count
 * @param renderer The SDL renderer to draw the grid lines
 */
void ObjectManager::drawGridLines()
{
    int column_count = 10;
    int row_count = 10;
    int cell_width = screen_width / column_count;
    int cell_height = screen_height / row_count;
    

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
    // Destruct the particles and walls
    delete[] particles;
}
#include "Particle.h"
#include <glad/glad.h>
#include <imgui.h>

void Particle::updatePosition(double delta)
{
	// Save the old position
	old_position.x = position.x;
	old_position.y = position.y;

	// Update position based on velocity and angle using elapsed time
	position.x += (p_velocity * cos(p_angle) * delta);
	position.y += (p_velocity * sin(p_angle) * delta);
} 

void Particle::handleScreenCollision()
{
	// Distance from the particle's old position to the new position
	double distance = sqrt(pow(position.x - old_position.x, 2) + pow(position.y - old_position.y, 2));
	distance += radius;

	// Clamping the particle's position to the screen
	if (position.x < 0) { position.x = radius; }
	else if (position.x > screen_width) { position.x = screen_width - radius; }

	if (position.y < 0) { position.y = radius; }
	else if (position.y > screen_height) { position.y = screen_height - radius; }

	// Bounce off the walls
	if (position.x <= radius || position.x >= screen_width - radius) {
		// Reflect the particle's angle against a vertical wall
		p_angle = M_PI - p_angle;
		p_angle = normalizeAngle(p_angle);

		// Move x by the distance
		position.x += (int)(distance * cos(p_angle));
	}

	if (position.y <= radius || position.y >= screen_height - radius) {
		// Reflect the particle's angle against a horizontal wall
		p_angle = -p_angle;
		p_angle = normalizeAngle(p_angle);

		// Move y by the distance
		position.y += (int)(distance * sin(p_angle));
	}
}

/**
 * Handles the particles collision given a line by checking if
 * there is an intersection between the particle and the line
 * @param line The line to check for collision
 * @return True if there is a collision, false otherwise
 */
bool Particle::handleLineCollision(Line line)
{
	// Check if particle's old and new position intersects with the line
	bool collided = false;

	Line particle_line = {old_position, position};

	Position intersection = lineIntersection(line, particle_line);

	if (intersection.x != 0 && intersection.y != 0)
	{
		collided = true;
	}

	// Distance of new position to intersection point
	int distance = (int)sqrt(pow(position.x - intersection.x, 2) + pow(position.y - intersection.y, 2));
	

	// Update the particle's position to the intersection point and adjust the angle
	if (collided)
	{
		// Update the particle's position to the intersection point
		position.x = intersection.x;
		position.y = intersection.y;

		// Reflect the particle's angle based on the line's angle in Radians with a simple reflection formula
		p_angle = reflectAngle(p_angle, line.angle);

		// Add radius to distance
		distance += radius + 2;
		
		// And then move the position by the distance
		position.x += (int)(distance * cos(p_angle));
		position.y += (int)(distance * sin(p_angle));

		// To offset radius

		return true;
	}

	return false;
}

/**
 * @brief Draws the particle
 * @details This function draws the particle
 */
void Particle::draw(SDL_Renderer* renderer) const
{
	// Draws a white square at the particle's position

	// Set color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	int round_x = (int)position.x;
	int round_y = (int)position.y;

	// Draw a Square
	SDL_Rect rect = {round_x - radius, round_y - radius, radius * 2, radius * 2};
	SDL_RenderFillRect(renderer, &rect);
}

/**
 * Draw the particle if within 33x19 rectangle of the player
 * @param renderer The SDL renderer to draw the particle
 * @param player_pos The player's position
 */
void Particle::drawExplorer(SDL_Renderer* renderer, Position player_pos) const
{
	// Draw the particle if within 33x19 rectangle of the player
	if (position.x >= player_pos.x - 16 && position.x <= player_pos.x + 16 && position.y >= player_pos.y - 9 && position.y <= player_pos.y + 9)
	{
		// Set color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		int round_x = (int)position.x;
		int round_y = (int)position.y;

		// Draw a Square but translate based on player's position (640, 360)
		// because we changed view from 1280x720 to 33x19
		// Assume each pixel is 38.7879x37.8947
		int new_draw_pos_x = (int)(640 + (round_x - player_pos.x) * 38.7879);
		int new_draw_pos_y = (int)(360 + (round_y - player_pos.y) * 37.8947);
		int width = (int)(38.7879);
		int height = (int)(37.8947);

		SDL_Rect rect;
		rect.x = new_draw_pos_x;
		rect.y = new_draw_pos_y;
		rect.w = width;
		rect.h = height;

		SDL_RenderFillRect(renderer, &rect);
	}
}

Particle::Particle(int id, int x, int y) {
	m_id = id;
	position.x = x;
	position.y = y;
	old_position.x = x;
	old_position.y = y;
	p_angle = 0;
	p_velocity = 0;
}

Particle::Particle(int id, int x, int y, double angle, double velocity)
{
	m_id = id;
	position.x = x;
	position.y = y;
	old_position.x = x;
	old_position.y = y;
	p_angle = angle;
	p_velocity = velocity;

	// Fix angle to counter-clockwise to match the coordinate system
	p_angle = 360 - p_angle;
	if (p_angle == 360) { p_angle = 0; }

	// Convert degrees to radians
	p_angle = p_angle * M_PI / 180;

	// Normalize angle
	p_angle = normalizeAngle(p_angle);

	// Fix velocity to pixels per second
	p_velocity = (double)velocity;
}

Particle::Particle()
{
	m_id = -1;
	position.x = 0;
	position.y = 0;
	old_position.x = 0;
	old_position.y = 0;
	p_angle = 0;
	p_velocity = 0;
}

Particle::~Particle()
{
	// TODO: Implement destructor
}
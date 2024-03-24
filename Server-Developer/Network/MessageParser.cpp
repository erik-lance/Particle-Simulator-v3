#include "MessageParser.h"

void MessageParser::parseMessage(char* message)
{
}

/**
 * Adds a player to the game to all clients based on message
 * @param id The player's ID
 * @param position The player's position
 */
void MessageParser::addPlayerMessage(char* id, Position position)
{
}

void MessageParser::movePlayerMessage(Position position, int direction)
{
}

/**
 * Adds a particle to the game to all clients based on message
 * @param position The particle's position
 * @param angle The particle's angle
 * @param velocity The particle's velocity
 */
void MessageParser::addParticleMessage(Position position, double angle, double velocity)
{
	char* message = new char[128];
	sprintf(message, "<p>%f,%f,%f,%f</p>", position.x, position.y, angle, velocity);
}

/**
 * Adds a particle batch to the game to all clients based on message
 * @param num The number of particles in the batch
 * @param start The starting position of the particles
 * @param end The ending position of the particles
 * @param angle The angle of the particles
 * @param velocity The velocity of the particles
 */
void MessageParser::addParticleBatchMessageOne(int num, Position start, Position end, double angle, double velocity)
{
	char* message = new char[128];
	sprintf(message, "<b>1,%d,%f,%f,%f,%f,%f,%f</b>", num, start.x, end.x, start.y, end.y, angle, velocity);
}

/**
 * Adds a particle batch to the game to all clients based on message
 * @param num The number of particles in the batch
 * @param start_angle The starting angle of the particles
 * @param end_angle The ending angle of the particles
 * @param start The starting position of the particles
 * @param velocity The velocity of the particles
 */
void MessageParser::addParticleBatchMessageTwo(int num, double start_angle, double end_angle, Position start, double velocity)
{
	char* message = new char[128];
	sprintf(message, "<b>2,%d,%f,%f,%f,%f,%f</b>", num, start_angle, end_angle, start.x, start.y, velocity);
}

/**
 * Adds a particle batch to the game to all clients based on message
 * @param num The number of particles in the batch
 * @param start The starting position of the particles
 * @param angle The angle of the particles
 * @param start_velocity The starting velocity of the particles
 * @param end_velocity The ending velocity of the particles
 */
void MessageParser::addParticleBatchMessageThree(int num, Position start, double angle, double start_velocity, double end_velocity)
{
	char* message = new char[128];
	sprintf(message, "<b>3,%d,%f,%f,%f,%f,%f</b>", num, start.x, start.y, angle, start_velocity, end_velocity);
}

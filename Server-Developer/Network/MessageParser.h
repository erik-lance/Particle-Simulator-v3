#pragma once
#include "../Structures.h"
#include "Server.h"
#include "../Objects/ObjectManager.h"

class MessageParser {
public:
	MessageParser(Server* server, ObjectManager* object_manager) : server(server), object_manager(object_manager) {}
	MessageParser() {}
	~MessageParser() {}

	void parseMessage(char* message);

	void addPlayerMessage(char* id, Position position);
	void movePlayerMessage(Position position, int direction);

	void addParticleMessage(Position position, double angle, double velocity);
	void addParticleBatchMessageOne(int num, Position start, Position end, double angle, double velocity);
	void addParticleBatchMessageTwo(int num, double start_angle, double end_angle, Position start, double velocity);
	void addParticleBatchMessageThree(int num, Position start, double angle, double start_velocity, double end_velocity);
private:
	Server* server;
	ObjectManager* object_manager;
};
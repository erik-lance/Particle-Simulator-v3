#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "../Structures.h"
#include <iostream>
#include <string>

class Player
{
public:
	Player(std::string ID, Position pos);
	~Player();

	Position getPosition() const { return position; }
	std::string getUUID() const { return UUID; }

	void setPosition(Position pos) { position = pos; }
	void updateDirection(Position dir) { currentDirection = dir; }
	void move(double deltaTime);
	void loadSpriteFromNumber(SDL_Renderer* renderer, int num);
	void draw(SDL_Renderer* renderer) const;
	void place(Position pos) { position = pos; }
private:
	std::string UUID;
	Position position;
	Position currentDirection = { 0, 0 };
	SDL_Texture* sprite = NULL;
	int moveSpeed = 100;
	int spriteWidth = 32;
	int spriteHeight = 32;
};
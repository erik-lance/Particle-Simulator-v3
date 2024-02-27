#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "../Structures.h"
#include <iostream>

class Player
{
public:
	Player(Position pos);
	~Player();

	Position getPosition() const { return position; }

	void setPosition(Position pos) { position = pos; }
	void move(int dir, double deltaTime);
	bool loadSprite(SDL_Renderer* renderer);
	bool loadSpecifiedSprite(SDL_Renderer* renderer, const char* path);
	void draw(SDL_Renderer* renderer) const;
private:
	Position position;
	SDL_Texture* sprite = NULL;
	int moveSpeed = 500;
};
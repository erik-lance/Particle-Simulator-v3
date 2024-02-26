#pragma once
#include <SDL.h>
#include "../Structures.h"

class Player
{
public:
	Player(Position pos);
	~Player();

	Position getPosition() const { return position; }

	void setPosition(Position pos) { position = pos; }
	void updatePosition(double delta);
	void draw(SDL_Renderer* renderer) const;
private:
	Position position;

};
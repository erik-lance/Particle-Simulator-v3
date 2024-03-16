#pragma once
#include "../Structures.h"
#include <SDL.h>

class DebugCircle
{
public:
	DebugCircle();
	DebugCircle(Position pos, int r);
	~DebugCircle();

	Position getPosition() const { return position; }
	int getRadius() const { return radius; }

	void setPosition(Position pos) { position = pos; }
	void setRadius(int r) { radius = r; }

	void draw(SDL_Renderer* renderer) const;

private:
	Position position;
	int radius = 3;
};

#include "DebugCircle.h"

DebugCircle::DebugCircle()
{
	Position newPos = { -1, -1 };
	position = newPos;
	radius = 3;
}

DebugCircle::DebugCircle(Position pos, int r)
{
	position = pos;
	radius = r;
}

DebugCircle::~DebugCircle()
{
}

void DebugCircle::draw(SDL_Renderer* renderer) const
{
	if (position.x == -1 && position.y == -1) { return; }
	// Set color to red
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	// Draw a circle
	for (int i = 0; i < 360; i++)
	{
		float x = position.x + radius * cos(i);
		float y = position.y + radius * sin(i);
		SDL_RenderDrawPoint(renderer, x, y);
	}
}

#include "Wall.h"

Wall::Wall(int id) {
	m_id = id;
	m_line = Line();
}

Wall::Wall() {
	m_id = 0;
	m_line = Line();
}

Wall::~Wall() {
}

void Wall::draw(SDL_Renderer* renderer)
{
	// Set color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Draw the line
	SDL_RenderDrawLine(renderer, (int)m_line.start.x, (int)m_line.start.y, (int)m_line.end.x, (int)m_line.end.y);
}


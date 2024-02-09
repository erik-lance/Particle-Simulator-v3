#pragma once
#include "../Structures.h"
#include <SDL.h>

class Wall {
public:
	Wall(int id);
	Wall();
	Wall(Line line) { m_line = line; }
	~Wall();

	int getID() const { return m_id; }
	Line getLine() const { return m_line; }

	void setID(int id) { m_id = id; }
	void draw(SDL_Renderer* renderer);

private:
	int m_id = -1;
	Line m_line;

};

#pragma once

struct Position { int x; int y; };
struct Line  { Position start; Position end; };
struct Screen { int width = 1280; int height = 720; };

bool operator==(const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

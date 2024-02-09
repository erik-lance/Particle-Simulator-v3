#pragma once

struct Position { int x; int y; };
struct Line  { Position start; Position end; };
struct Screen { int width = 1280; int height = 720; };

bool operator==(const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

// Line intersection
bool lineIntersectsLine(Line l1, Line l2)
{
    // Get the segments' parameters
    float dx12 = l1.end.x - l1.start.x;
    float dy12 = l1.end.y - l1.start.y;
    float dx34 = l2.end.x - l2.start.x;
    float dy34 = l2.end.y - l2.start.y;

    // Solve for t1 and t2
    float denominator = (dy12 * dx34 - dx12 * dy34); // This is the determinant of the matrix formed by the lines' vectors

    // Check if the lines are parallel
    if (denominator == 0) {
        // Lines are parallel
        return false;
    }

    float t1 = ((l1.start.x - l2.start.x) * dy34 + (l2.start.y - l1.start.y) * dx34) / denominator;
    float t2 = ((l2.start.x - l1.start.x) * dy12 + (l1.start.y - l2.start.y) * dx12) / (-denominator);

    // Check if the line segments intersect
    if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
        // Line segments intersect
        return true;
    }

    // Line segments do not intersect
    return false;
}

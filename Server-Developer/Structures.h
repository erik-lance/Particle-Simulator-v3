#pragma once

// PI Constant
constexpr auto PI = 3.14159265358979323846;

struct Position { double x; double y; };
struct Line { Position start; Position end; double angle; };
struct Screen { int width = 1280; int height = 720; };

static inline bool operator==(const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

// Line intersection
static inline bool lineIntersectsLine(Line l1, Line l2)
{
    // Get the segments' parameters
    float dx12 = (float)l1.end.x - (float)l1.start.x;
    float dy12 = (float)l1.end.y - (float)l1.start.y;
    float dx34 = (float)l2.end.x - (float)l2.start.x;
    float dy34 = (float)l2.end.y - (float)l2.start.y;

    // Solve for t1 and t2
    float denominator = (dy12 * dx34 - dx12 * dy34); // This is the determinant of the matrix formed by the lines' vectors

    // Check if the lines are parallel
    if (denominator == 0) {
        // Lines are parallel
        return false;
    }

    float t1 = ((float)(l1.start.x - l2.start.x) * dy34 + (float)(l2.start.y - l1.start.y) * dx34) / denominator;
    float t2 = ((float)(l2.start.x - l1.start.x) * dy12 + (float)(l1.start.y - l2.start.y) * dx12) / (-denominator);

    /*
    std::cout << "Line 1: (" << l1.start.x << ", " << l1.start.y << ") to (" << l1.end.x << ", " << l1.end.y << ")" << std::endl;
    std::cout << "Line 2: (" << l2.start.x << ", " << l2.start.y << ") to (" << l2.end.x << ", " << l2.end.y << ")" << std::endl;

    std::cout << "dx12: " << dx12 << std::endl;
    std::cout << "dy12: " << dy12 << std::endl;
    std::cout << "dx34: " << dx34 << std::endl;
    std::cout << "dy34: " << dy34 << std::endl;
    */

    // Check if the line segments intersect
    if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
        // Line segments intersect
        return true;
    }

    // Line segments do not intersect
    return false;
}

static inline Position lineIntersection(Line l1, Line l2)
{
    // Get the segments' parameters
    double dx12 = l1.end.x - l1.start.x;
    double dy12 = l1.end.y - l1.start.y;
    double dx34 = l2.end.x - l2.start.x;
    double dy34 = l2.end.y - l2.start.y;

    // Solve for t1 and t2
    double denominator = (dy12 * dx34 - dx12 * dy34); // This is the determinant of the matrix formed by the lines' vectors

    // Check if the lines are parallel
    if (denominator == 0) {
		// Lines are parallel
		return { 0, 0 }; // Or some other indication of no intersection
	}

    double t1 = ((l1.start.x - l2.start.x) * dy34 + (l2.start.y - l1.start.y) * dx34) / denominator;
    double t2 = ((l2.start.x - l1.start.x) * dy12 + (l1.start.y - l2.start.y) * dx12) / (-denominator);

    // Check if the line segments intersect
    if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
		// Line segments intersect
		return { l1.start.x + dx12 * t1, l1.start.y + dy12 * t1 };
	}

    // Line segments do not intersect
    return { 0, 0 }; // Or some other indication of no intersection
}

// Function to normalize angle in Radians [0, 2pi]
static inline double normalizeAngle(double angle)
{
    // Normalize the angle to be between 0 and 2pi
    while (angle < 0) { angle += 2.0 * PI; }
    while (angle > 2 * PI) { angle -= 2.0 * PI; }
    return angle;
}

// Function to reflect angle in Radians
static inline double reflectAngle(double angle, double line_angle)
{
    // Reflection formula
    angle = 2 * line_angle - angle;

    angle = normalizeAngle(angle);
	return angle;
}
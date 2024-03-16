#pragma once

#include "Structures.h"
#include "Objects/Particle.h"
#include "Objects/Wall.h"

// Represents a cell in the grid
struct Cell {
	Position position;   // Position of the cell
	int cell_width;      // Width of the cell
	int cell_height;     // Height of the cell
	Particle* particles; // Array of particles in this cell
	Wall* walls;         // Array of walls in this cell
	int numParticles;    // Number of particles in this cell
	int numWalls;        // Number of walls in this cell
	int maxParticles;    // Maximum number of particles in this cell
	int maxWalls;        // Maximum number of walls in this cell
};

static inline bool operator==(const Cell& c1, const Cell& c2) { return c1.position == c2.position; }

// Represents the grid
struct Grid {
	int columns;
	int rows;
	int cell_width;
	int cell_height;
	Cell** cells; // 2D array of cells
};

/**
 * The CollisionManager splits the simulation into a grid and checks for collisions within the grid.
 * If a particle is in the same cell as a line, it is then able to check for collisions as long as
 * they are in the same cell. This will reduce the number of collision checks and improve performance.
 */
class CollisionManager {
public:
	CollisionManager(int width, int height);
	CollisionManager(int width, int height, int columns, int rows);
	~CollisionManager();

	void addWall(Wall wall);
	bool cellIntersectsLine(Cell cell, Line line);

	void setSimulatorDimensions(int width, int height) { simulator_width = width; simulator_height = height; }
	void setGridDimensions(int columns, int rows);

	int getGridColumns() const { return grid_columns; }
	int getGridRows() const { return grid_rows; }
	int getGridCellWidth() const { return grid_cell_width; }
	int getGridCellHeight() const { return grid_cell_height; }

	Cell getGridCell(int x, int y) const;
	void checkParticleCollisionsInCells(Particle* particle) const;

private:
	int simulator_width, simulator_height;
	int grid_columns, grid_rows;
	int grid_cell_width, grid_cell_height;

	// Array Grid of Wall and Particle IDs on each cell
	// Multiple particles and lines can be in the same cell
	Grid grid;

};

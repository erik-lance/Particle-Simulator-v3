#include "CollisionManager.h"

CollisionManager::CollisionManager(int width, int height)
{
	setSimulatorDimensions(width, height);
	setGridDimensions(10, 10);
}

CollisionManager::CollisionManager(int width, int height, int columns, int rows)
{
	setSimulatorDimensions(width, height);
	setGridDimensions(columns, rows);
}

CollisionManager::~CollisionManager()
{
	// Deallocate memory for the 2D array of cells in the grid
	for (int i = 0; i < grid_columns; i++)
	{
		// Deallocate array of particles and walls in each cell
		for (int j = 0; j < grid_rows; j++)
		{
			delete[] grid.cells[i][j].particles;
			delete[] grid.cells[i][j].walls;
		}
	}
}

/**
 * Adds wall to cell/s in the grid.
 * @param wall The wall to add to the grid
 */
void CollisionManager::addWall(Wall wall)
{
	// Since wall can span multiple cells, add the wall to the cells it intersects
	Position start = wall.getLine().start;
	Position end = wall.getLine().end;

	Cell start_cell = getGridCell(start.x, start.y);
	Cell end_cell = getGridCell(end.x, end.y);

	// Check a quadrilateral formed by the start and end cell
	// and add the wall to the cells it intersects

	for (int i = start_cell.position.x; i <= end_cell.position.x; i++)
	{
		for (int j = start_cell.position.y; j <= end_cell.position.y; j++)
		{
			// Check if line intersects the cell
			// If it does, add the wall to the cell
			Cell cell = grid.cells[i][j];
			bool intersects = false;

			// Check if start or end cell because it is automatically intersected
			if (cell == start_cell || cell == end_cell) { intersects = true; }
			else
			{
				// Check if the line intersects the cell
				Line line = wall.getLine();
				intersects = cellIntersectsLine(cell, line);
			}

			if (intersects)
			{
				// Add the wall to the cell. Allocate memory
				// if the cell's walls array is full
				if (cell.numWalls >= cell.maxWalls)
				{
					cell.maxWalls *= 2;
					Wall* new_walls = new Wall[cell.maxWalls];
					for (int i = 0; i < cell.numWalls-1; i++)
					{
						new_walls[i] = cell.walls[i];
					}
					delete[] cell.walls;
					cell.walls = new_walls;
				}

				// Add the wall to the cell
				cell.walls[cell.numWalls-1] = wall;
				cell.numWalls++;

				grid.cells[i][j] = cell;
			}
		}
	}
}

/**
 * Checks if a cell intersects with a line
 * @param cell The cell to check for intersection
 * @param line The line to check for intersection
 */
bool CollisionManager::cellIntersectsLine(Cell cell, Line line)
{
	// Check if the line intersects the cell
	bool intersects = false;

	// Check each line of the cell for intersection
	// Cell position is the position on the grid. So we have to multiply
	// the cell position by the cell width and height to get the actual
	Line lines[4] = {
		{{cell.position.x * grid_cell_width, cell.position.y * grid_cell_height}, {cell.position.x * grid_cell_width + grid_cell_width, cell.position.y * grid_cell_height}},
		{{cell.position.x * grid_cell_width + grid_cell_width, cell.position.y * grid_cell_height}, {cell.position.x * grid_cell_width + grid_cell_width, cell.position.y * grid_cell_height + grid_cell_height}},
		{{cell.position.x * grid_cell_width + grid_cell_width, cell.position.y * grid_cell_height + grid_cell_height}, {cell.position.x * grid_cell_width, cell.position.y * grid_cell_height + grid_cell_height}},
		{{cell.position.x * grid_cell_width, cell.position.y * grid_cell_height + grid_cell_height}, {cell.position.x * grid_cell_width, cell.position.y * grid_cell_height}}
	};

	for (int i = 0; i < 4; i++)
	{
		// Check for intersection
		if (lineIntersectsLine(lines[i], line))
		{
			intersects = true;
			break;
		}
	}

	return intersects;
}

/**
 * Sets up the grid dimensions, cell sizes, and initializes the grid
 * @param columns The number of columns in the grid
 * @param rows The number of rows in the grid
 */
void CollisionManager::setGridDimensions(int columns, int rows)
{
	grid_columns = columns;
	grid_rows = rows;

	grid_cell_width = simulator_width / grid_columns;
	grid_cell_height = simulator_height / grid_rows;

	// Initialize the grid
	grid.columns = grid_columns;
	grid.rows = grid_rows;
	grid.cell_width = grid_cell_width;
	grid.cell_height = grid_cell_height;

	// Allocate memory for the grid
	grid.cells = new Cell * [grid_columns];
	for (int i = 0; i < grid_columns; i++)
	{
		grid.cells[i] = new Cell[grid_rows];
	}

	// Initialize the cells
	for (int i = 0; i < grid_columns; i++)
	{
		for (int j = 0; j < grid_rows; j++)
		{
			Position pos{ i, j };
			grid.cells[i][j].position = pos;
			grid.cells[i][j].cell_width = grid_cell_width;
			grid.cells[i][j].cell_height = grid_cell_height;
			grid.cells[i][j].maxParticles = 1024;
			grid.cells[i][j].maxWalls = 1024;
			grid.cells[i][j].numParticles = 1;
			grid.cells[i][j].numWalls = 1;
			grid.cells[i][j].particles = new Particle[grid.cells[i][j].maxParticles];
			grid.cells[i][j].walls = new Wall[grid.cells[i][j].maxWalls];
		}
	}
}

/**
 * Gets the grid cell given the particle's position
 * @param x The particle's x position
 * @param y The particle's y position
 * @return A pair of integers representing the grid cell
 */
Cell CollisionManager::getGridCell(int x, int y) const
{
	// Calculates the particle position to correspond to the grid
	int cell_x = x / grid_cell_width;
	int cell_y = y / grid_cell_height;

	// Clamp the cell to the grid
	if (cell_x < 0) cell_x = 0;
	if (cell_x >= grid_columns) cell_x = grid_columns - 1;
	if (cell_y < 0) cell_y = 0;
	if (cell_y >= grid_rows) cell_y = grid_rows - 1;

	// Return the cell from grid
	return grid.cells[cell_x][cell_y];
}

/**
 * Checks for a particle's collisions in a start and end cell to see
 * if its start-end position intersects with any wall in the two cells.
 * @param particle The particle to check for collisions
*/
void CollisionManager::checkParticleCollisionsInCells(Particle* particle) const
{
	Position start_pos = particle->getOldPosition();
	Position end_pos = particle->getPosition();

	Cell start_cell = getGridCell(start_pos.x, start_pos.y);
	Cell end_cell = getGridCell(end_pos.x, end_pos.y);

	// Check for collisions between the particle and the walls in the start and end cells

	// Check for collisions in the start cell
	bool done = false;
	for (int i = 0; i < start_cell.numWalls-1; i++)
	{
		// Get the wall
		Wall wall = start_cell.walls[i];

		// Check for collisions
		bool collided = particle->handleLineCollision(wall.getLine());

		// If there is a collision, stop checking for collisions for this particle
		if (collided) { done = true; break; }
	}

	if (done) return;

	// Check for collisions in the end cell
	for (int i = 0; i < end_cell.numWalls-1; i++)
	{
		// Get the wall
		Wall wall = end_cell.walls[i];

		// Check for collisions
		bool collided = particle->handleLineCollision(wall.getLine());

		// If there is a collision, stop checking for collisions for this particle
		if (collided) { done = true; break; }
	}
}

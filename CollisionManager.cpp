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
	grid.resize(grid_columns);
	for (int i = 0; i < grid_columns; i++)
	{
		grid[i].resize(grid_rows);
		for (int j = 0; j < grid_rows; j++)
		{
			grid[i][j].resize(2); // Initialize the grid cell to have 2 elements (particle and line) to hold IDs
		}
	}
}

/**
 * Gets the grid cell given the particle's position
 * @param x The particle's x position
 * @param y The particle's y position
 * @return A pair of integers representing the grid cell
 */
std::pair<int, int> CollisionManager::getGridCell(int x, int y) 
{
	// Calculates the particle position to correspond to the grid
	int cell_x = x / grid_cell_width;
	int cell_y = y / grid_cell_height;

	return std::make_pair(cell_x, cell_y);
}

#include "CollisionManager.h"

CollisionManager::CollisionManager(int width, int height)
	: simulator_width(width), simulator_height(height)
{

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
}

/**
 * Gets the grid cell given the particle's position
 * @param x The particle's x position
 * @param y The particle's y position
 */
std::pair<int, int> CollisionManager::getGridCell(int x, int y) 
{

}

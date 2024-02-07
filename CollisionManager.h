#pragma once
#include <vector>

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

	void setSimulatorDimensions(int width, int height) { simulator_width = width; simulator_height = height; }
	void setGridDimensions(int columns, int rows);

	std::pair<int, int> getGridCell(int x, int y);

private:
	int simulator_width, simulator_height;
	int grid_columns, grid_rows;
	int grid_cell_width, grid_cell_height;

	// Grid
	std::vector<std::vector<int>> grid;
};

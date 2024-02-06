#pragma once

/**
 * @brief CollisionManager class
 * @details This class is used to manage collisions
 */
class CollisionManager {
public:
	CollisionManager(int width, int height);
	~CollisionManager();

	void setSimulatorDimensions(int width, int height) { simulator_width = width; simulator_height = height; }
	void setGridDimensions(int columns, int rows) { grid_columns = columns; grid_rows = rows; }
	void setGridCellDimensions(int width, int height) { grid_cell_width = width; grid_cell_height = height; }

private:
	int simulator_width, simulator_height;
	int grid_columns, grid_rows;
	int grid_cell_width, grid_cell_height;

};

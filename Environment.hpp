#pragma once
#include <vector>
#include <queue>

enum class CellType {
    OutOfBounds = -1,
    EMPTY = 0,
    WALL = 1,
    START = 2,
    END = 3
};

class Environment {
private:
    std::vector<std::vector<CellType>> grid;
    int width;
    int height;

public:
    Environment(int w, int h);                                                 // Constructor to initialize the environment with given width and height

    void setCell(int x, int y, CellType value);                                // Set the cell at (x, y) to the specified CellType value

    void setGrid(const std::vector<std::vector<CellType>>& newGrid);           // Set the entire grid to a new configuration

    CellType getCell(int x, int y) const;                                      // Get the CellType of the cell at (x, y), returns OutOfBounds if (x, y) is outside the grid

    int getWidth() const;                                                      // Get the width of the environment  
    int getHeight() const;                                                     // Get the height of the environment

    bool isReachable(int startX, int startY, int endX, int endY);              // Check if the END cell is reachable from the START cell using a simple BFS or DFS algorithm
};
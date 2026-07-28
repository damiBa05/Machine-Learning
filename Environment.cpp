#include "Environment.hpp"

Environment::Environment(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
}

void Environment::setCell(int x, int y, CellType value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = value;
    }
}

void Environment::setGrid(const std::vector<std::vector<CellType>>& newGrid) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                grid[y][x] = newGrid[y][x];
            }
        }
    }

CellType Environment::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return CellType::OutOfBounds;
}

int Environment::getWidth() const { return width; }
int Environment::getHeight() const { return height; }
#pragma once
#include <vector>

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
    Environment(int w, int h);

    void setCell(int x, int y, CellType value);

    void setGrid(const std::vector<std::vector<CellType>>& newGrid);

    CellType getCell(int x, int y) const;

    int getWidth() const;
    int getHeight() const;

};
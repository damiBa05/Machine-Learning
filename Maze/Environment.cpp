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

bool Environment::isReachable(int startX, int startY, int endX, int endY) {
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<std::pair<int, int>> toVisit;

    toVisit.push({ startX, startY });
    visited[startY][startX] = true;

    const int dx[4] = { 0, 0, -1, 1 };
    const int dy[4] = { -1, 1, 0, 0 };

    while (!toVisit.empty()) {
        auto [x, y] = toVisit.front();
        toVisit.pop();

        if (x == endX && y == endY) {
            return true;
        }

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
            if (visited[ny][nx]) continue;
            if (getCell(nx, ny) == CellType::WALL) continue;

            visited[ny][nx] = true;
            toVisit.push({ nx, ny });
        }
    }
    return false; // never reached endX, endY
}
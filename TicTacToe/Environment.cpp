#include "Environment.hpp"

Environment::Environment(int x, int y) : height(y), width(x) {
    board.resize(height, std::vector<BoardState>(width, BoardState::Empty));
}

void Environment::setCell(int x, int y, BoardState state) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        board[y][x] = state;
    }
}

void Environment::cleanBoard() {
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), BoardState::Empty);
    }
}

GameResult Environment::getGameResult() const {
    //The tic-tac-toe board has 8 possible winning lines: 3 rows, 3 columns, and 2 diagonals.
    //This is designed so it can check faster and safer the results
    static const int lines[8][3][2] = {
        {{0,0},{0,1},{0,2}}, // rows
        {{1,0},{1,1},{1,2}},
        {{2,0},{2,1},{2,2}},
        {{0,0},{1,0},{2,0}}, // columns
        {{0,1},{1,1},{2,1}},
        {{0,2},{1,2},{2,2}},
        {{0,0},{1,1},{2,2}}, // diagonal
        {{0,2},{1,1},{2,0}}  // anti-diagonal
    };

    for (const auto& line : lines) {
        BoardState a = board[line[0][0]][line[0][1]];
        BoardState b = board[line[1][0]][line[1][1]];
        BoardState c = board[line[2][0]][line[2][1]];
        if (a != BoardState::Empty && a == b && b == c) {
            return a == BoardState::X ? GameResult::XWins : GameResult::OWins;
        }
    }

    for (const auto& row : board)
        for (const auto& cell : row)
            if (cell == BoardState::Empty) return GameResult::InProgress;

    return GameResult::Draw;
}

BoardState Environment::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return board[y][x];
    }
    return BoardState::Empty; // Return Empty for out-of-bounds access
}

int Environment::getWidth() const {
    return width;
}

int Environment::getHeight() const {
    return height;
}

bool Environment::isValidPlacement(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < height && board[y][x] == BoardState::Empty);
}

void Environment::printBoard() const {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            char mark = ' ';
            if (cell == BoardState::X) mark = 'X';
            else if (cell == BoardState::O) mark = 'O';
            std::cout << mark << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
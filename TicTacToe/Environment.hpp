#pragma once
#include <vector>

enum class BoardState{                              //Enum to help with the state of the board
    Empty = 0,
    X = 1,
    O = 2
};
enum class GameResult { InProgress, XWins, OWins, Draw };

class Environment
{
private:
    std::vector<std::vector<BoardState>> board;
    int height;
    int width;
public:
    Environment(int x, int y);                      //Constuctor of the board9078=

    void setCell(int x, int y, BoardState state);   //To change single cells
    void cleanBoard();                              //Reset of the board
    GameResult getGameResult() const;
    BoardState getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
};

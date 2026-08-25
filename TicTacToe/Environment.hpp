#pragma once
#include <vector>
#include <iostream>

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
    GameResult getGameResult() const;               //To get the result of the game
    BoardState getCell(int x, int y) const;         //To get the state of a single cell
    int getWidth() const;                           //To get the width of the board
    int getHeight() const;                          //To get the height of the board
    bool isValidPlacement(int x, int y) const;      //To check if a placement is valid
    void printBoard() const;                          //To print the board
};

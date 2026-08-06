#pragma once

enum class PlayerMark{
    X = 0,
    O = 1
};

class Agent{
private:
int id;
int score;
PlayerMark mark;
bool gameState;

public:
Agent(int id, int score, PlayerMark playMark);
void setScore(int newScore);
void addScore(int delta);
int getScore();
bool setGame();
bool isDone();
void reset();

};
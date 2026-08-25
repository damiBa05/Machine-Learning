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
Agent(int id, int score, PlayerMark playMark);  //Constructor of the agent
void setScore(int newScore);                    //To set the score of the agent
void addScore(int delta);                       //To add to the score of the agent
int getScore();                                 //To get the score of the agent
bool setGame();                                 //To set the game state of the agent
bool isDone();                                  //To check if the game is done for the agent
void reset();                                   //To reset the game state of the agent
PlayerMark getMark() const;                     //To get the mark of the agent

};
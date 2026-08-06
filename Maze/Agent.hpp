#pragma once

class Agent{
private:
    int id;
    int score;
    int positionX;
    int positionY;
    bool isDone; // New member variable to track if the agent has reached the goal

public:
    Agent(int agentId, int startX, int startY);     //Constructor
    int getId() const;                              // Getter for agent ID
    int getScore() const;                           // Getter for agent score   
    void setScore(int newScore);                    // Setter for agent score
    void addScore(int delta);                       // Method to add to the agent's score
    int getPositionX() const;                       // Getter for agent's X position
    int getPositionY() const;                       // Getter for agent's Y position
    void setPositionX(int newX);                    // Setter for agent's X position
    void setPositionY(int newY);                    // Setter for agent's Y position
    bool getDone() const;                           // Getter for isDone
    void setDone(bool done);                        // Setter for isDone
    void reset(int startX, int startY);             // Method to reset the agent's state

};
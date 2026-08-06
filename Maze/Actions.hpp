#pragma once
#include "Agent.hpp"
#include "Environment.hpp"
#include <cstdlib>

class Actions {
public:
    bool static isValidMove(int newX, int newY, const Environment& env);                        // Check if the new position is within the grid boundaries
    bool static isWall(int x, int y, const Environment& env);                                   // Check if the new position is a wall  
    bool static isGoal(int x, int y, const Environment& env);                                   // Check if the new position is the goal
    static void move(Agent& agent, Environment& env, int dx, int dy);                           // Move the agent in the specified direction (dx, dy) and update its score based on the new position
    static int applySlip(int intendedAction, int numActions, double slipProbability);           // Apply slip to the intended action based on the slip probability and return the actual action taken

};
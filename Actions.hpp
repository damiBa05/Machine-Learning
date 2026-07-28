#pragma once
#include "Agent.hpp"
#include "Environment.hpp"

class Actions {
public:
    bool static isValidMove(int newX, int newY, const Environment& env);
    bool static isWall(int x, int y, const Environment& env);
    bool static isGoal(int x, int y, const Environment& env);
    static void move(Agent& agent, Environment& env, int dx, int dy);

};
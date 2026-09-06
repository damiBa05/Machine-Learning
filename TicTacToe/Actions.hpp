#pragma once
#include <vector>
#include <array>
#include "Environment.hpp"
#include "Agent.hpp"

class Actions
{
public:
    bool static isValidPlacement(int x, int y, const Environment& env);     //To ensure placement is valid
    static void place(Agent& agent, Environment& env, int x, int y);        //Place the mark
    std::array<int, 9> static getValidActions(const Environment& env);        //Gets the valid actions
};

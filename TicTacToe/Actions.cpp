#include "Actions.hpp"
#include "Environment.hpp"

bool Actions::isValidPlacement(int x, int y, const Environment& env) {
    return env.isValidPlacement(x, y);
}

void Actions::place(Agent& agent, Environment& env, int x, int y) {
    if (isValidPlacement(x, y, env)) {
        PlayerMark mark = agent.getMark();
        if (mark == PlayerMark::X) {
            env.setCell(x, y, BoardState::X);
        } else if (mark == PlayerMark::O) {
            env.setCell(x, y, BoardState::O);
        }
    }
}
std::vector<int> Actions::getValidActions(const Environment& env) {
    std::vector<int> validActions;
    for (int x = 0; x < env.getWidth(); ++x) {
        for (int y = 0; y < env.getHeight(); ++y) {
            if (isValidPlacement(x, y, env)) {
                validActions.push_back(x * env.getHeight() + y); // Store as a single integer
            }
        }
    }
    return validActions;
}
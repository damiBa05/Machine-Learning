#include "Actions.hpp"

bool Actions::isValidMove(int newX, int newY, const Environment& env) {
    return (newX >= 0 && newX < env.getWidth() && newY >= 0 && newY < env.getHeight());
};

bool Actions::isWall(int x, int y, const Environment& env) {
    return env.getCell(x, y) == CellType::WALL;
}

bool Actions::isGoal(int x, int y, const Environment& env) {
    return env.getCell(x, y) == CellType::END;
}

void Actions::move(Agent& agent, Environment& env, int dx, int dy) {
    if (agent.getDone()) return;
    
    int newX = agent.getPositionX() + dx;
    int newY = agent.getPositionY() + dy;

    if (!isValidMove(newX, newY, env)) {
        agent.addScore(-10); // treat boundary same as wall — see above
        return;
    }
    if (isWall(newX, newY, env)) {
        agent.addScore(-10);
        return;
    }
    agent.setPositionX(newX);
    agent.setPositionY(newY);
    if (isGoal(newX, newY, env)) {
        agent.addScore(100);
        agent.setDone(true);
    } else {
        agent.addScore(-1);
    }
}

int Actions::applySlip(int intendedAction, int numActions, double slipProbability) {
    double randNum = static_cast<double>(rand()) / RAND_MAX;
    if (randNum < slipProbability) {
        return rand() % numActions;
    }
    return intendedAction;
}
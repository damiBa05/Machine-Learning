#include "Agent.hpp"

Agent::Agent(int agentId, int startX, int startY) : 
    id(agentId), score(0), positionX(startX), positionY(startY), isDone(false) {}

int Agent::getId() const {
    return id;
}

int Agent::getScore() const {
    return score;
}

void Agent::setScore(int newScore) {
    score = newScore;
}

void Agent::addScore(int delta) {
    score += delta;
}

int Agent::getPositionX() const {
    return positionX;
}

int Agent::getPositionY() const {
    return positionY;
}

void Agent::setPositionX(int newX) {
    positionX = newX;
}

void Agent::setPositionY(int newY) {
    positionY = newY;
}

bool Agent::getDone() const { 
    return isDone; 
}

void Agent::setDone(bool done) {
    isDone = done;
}

void Agent::reset(int startX, int startY) {
        score = 0;
        positionX = startX;
        positionY = startY;
        isDone = false;
    }
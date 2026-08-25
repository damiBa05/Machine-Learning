#include "Agent.hpp"

Agent::Agent(int id, int score, PlayerMark playMark) : id(id), score(score), mark(playMark), gameState(false) {}
void Agent::setScore(int newScore) {
    score = newScore;
}
void Agent::addScore(int delta) {
    score += delta;
}
int Agent::getScore() {
    return score;
}
bool Agent::setGame() {
    gameState = true;
    return gameState;
}
bool Agent::isDone() {
    return gameState;
}
void Agent::reset() {
    gameState = false;
}

PlayerMark Agent::getMark() const {
    return mark;
}
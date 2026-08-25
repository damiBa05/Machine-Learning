#include "QTable.hpp"

QTable::QTable(int rows, int cols) : numRows(rows), numCols(cols) {
    qValues.resize(numRows, std::vector<double>(numCols, 0.0));
}

double QTable::getQValue(int row, int col) const {
    return qValues[row][col];
}

void QTable::setQValue(int row, int col, double value) {
    qValues[row][col] = value;
}

void QTable::updateQValue(int row, int col, double value) {
    qValues[row][col] += value;
}

//Q(s,a) = Q(s,a) + alpha * (reward + gamma * max(Q(s', all actions)) - Q(s,a))
//Bellman equation for updating Q-values in Q-learning
/*
s — the state you were in before taking this action. In tic-tac-toe terms: 
the board configuration right before your move.
a — the action you took from that state. Here: which cell (0-8) you placed your mark in.
Q(s, a) — the table's current belief about "how good is it to take action a from state s.
" This is literally one cell in your table — qValues[s][a].
r - the reward
s' ("s-prime") — the state you ended up in after taking action a. The board, one move later.
a' ("a-prime") — this doesn't refer to one specific action — it means "loop over every 
possible action available from s'," so you can find the best one.
max(Q(s', a')) — of every action you could take from the new state s', what's the highest Q-value among them?
 This is "my current best guess at how good things get from here onward," and it's the piece that lets reward 
 propagate backward across many moves
 α (alpha) — learning rate. How much this one new experience should move your existing belief
 . Small alpha = cautious, slow updates; large alpha = trust the newest experience heavily.
γ (gamma) — discount factor. How much you weight future reward (max(Q(s', a'))) versus caring
 only about the immediate r. High gamma (close to 1) means you care a lot about where this leads eventually
*/

void QTable::update(int state, int action, double reward, int nextState,
    double learningRate, double discountFactor) {
    // Find the maximum Q-value for the next state
    double maxNextQValue = *std::max_element(qValues[nextState].begin(), qValues[nextState].end());
    
    // Update the Q-value using the Q-learning formula
    qValues[state][action] += learningRate * (reward + discountFactor * maxNextQValue - qValues[state][action]);
}

void QTable::reset() {
    for (int row = 0; row < numRows; ++row)
        for (int col = 0; col < numCols; ++col)
            qValues[row][col] = 0.0;
}

int QTable::stateIndex(const std::vector<int>& cells) const {
    int index = 0;
    int power = 1;
    for (int i = 0; i < static_cast<int>(cells.size()); ++i) {
        index += cells[i] * power;
        power *= 3; // build up 3^i by repeated multiplication, one step per iteration
    }
    return index;
}

int QTable::selectAction(int state, double epsilon, const std::vector<int>& validActions) const {
    // Epsilon-greedy action selection
    if (static_cast<double>(rand()) / RAND_MAX < epsilon) {
        // Explore: choose a random valid action
        int randomIndex = rand() % validActions.size();
        return validActions[randomIndex];
    } else {
        // Exploit: choose the action with the highest Q-value
        int bestAction = validActions[0];
        double bestQValue = qValues[state][bestAction];
        
        for (int action : validActions) {
            if (qValues[state][action] > bestQValue) {
                bestQValue = qValues[state][action];
                bestAction = action;
            }
        }
        return bestAction;
    }
}
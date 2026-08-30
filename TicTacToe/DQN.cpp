#include "DQN.hpp"

DQN::DQN(int actions, int cells) : numActions(actions), numCells(cells){
    weights.resize(numActions, std::vector<double>(numCells, 0.0));
    bias.resize(numActions, 0.0);
}

double DQN::getQValue(int action, const std::vector<int>& cells) const{
    double q = bias[action];               // start from this action's bias
    for (int i = 0; i < numCells; ++i) {
        q += weights[action][i] * cells[i]; // add each cell's contribution
    }
    return q;
}

void DQN::update(int action, const std::vector<int>& cells, double reward,
    const std::vector<int>& nextCells, bool isTerminal,
    double learningRate, double discountFactor) {

    double maxNextQValue = 0.0;
    if (!isTerminal) {
        maxNextQValue = getQValue(0, nextCells);
        for (int a = 1; a < numActions; ++a) {
            double q = getQValue(a, nextCells);
            if (q > maxNextQValue) {
                maxNextQValue = q;
            }
        }
    }

    double currentQ = getQValue(action, cells);
    double tdError = reward + discountFactor * maxNextQValue - currentQ;

    for (int i = 0; i < numCells; ++i) {
        weights[action][i] += learningRate * tdError * cells[i];
    }
    bias[action] += learningRate * tdError;
}

int DQN::selectAction(const std::vector<int>& cells, double epsilon,
    const std::vector<int>& validActions) const{
    // Epsilon-greedy action selection
    if (static_cast<double>(rand()) / RAND_MAX < epsilon) {
        // Explore: choose a random valid action
        int randomIndex = rand() % validActions.size();
        return validActions[randomIndex];
    }else{
        int bestAction = validActions[0];
        double bestQValue = getQValue(bestAction, cells);
        for (int action : validActions){
            double q = getQValue(action, cells);;
            if (q > bestQValue){
                bestQValue = q;
                bestAction = action;
            }
        }
        return bestAction;
    }
}

void DQN::reset(){
    for(double& b : bias){
        b = 0.0;
    }
    for (std::vector<double>& weight : weights){
        for (double& value : weight){
            value = 0.0;
        }
    }
}
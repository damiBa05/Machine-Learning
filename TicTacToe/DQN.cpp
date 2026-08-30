#include "DQN.hpp"

DQN::DQN(int actions, int cells, int cellsHidden1) : numActions(actions),
 numCells(cells), numHidden1(cellsHidden1){
    weightsOutput.resize(numActions, std::vector<double>(numHidden1, 0.0));
    biasOutput.resize(numActions, 0.0);
    weightsLayer1.resize(numHidden1, std::vector<double>(numCells, 0.0));
    biasLayer1.resize(numHidden1, 0.0);
}

double DQN::getQValue(int action, const std::vector<int>& cells) const{
    double q = biasOutput[action];               // start from this action's bias
    for (int i = 0; i < numCells; ++i) {
        q += weightsOutput[action][i] * cells[i]; // add each cell's contribution
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
        weightsOutput[action][i] += learningRate * tdError * cells[i];
    }
    biasOutput[action] += learningRate * tdError;
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
    for(double& b : biasOutput){
        b = 0.0;
    }
    for (std::vector<double>& weight : weightsOutput){
        for (double& value : weight){
            value = 0.0;
        }
    }
}
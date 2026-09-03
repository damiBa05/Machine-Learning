#include "DQN.hpp"

DQN::DQN(int actions, int cells, int cellsHidden1) : numActions(actions),
 numCells(cells), numHidden1(cellsHidden1){
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(-0.5, 0.5);
    weightsOutput.resize(numActions, std::vector<double>(numHidden1, 0.0));
    biasOutput.resize(numActions, 0.0);
    weightsLayer1.resize(numHidden1, std::vector<double>(numCells));
    for (auto& row : weightsLayer1)
        for (auto& w : row)
            w = dist(rng);
    biasLayer1.resize(numHidden1, 0.0);
}

double DQN::getQValue(int action, const std::vector<int>& cells) const{
    //Hidden layer logic
    // Step 1: cells -> hidden layer, ReLU applied to each hidden neuron
    std::vector<double> hiddenOutput(numHidden1, 0.0);
    for(int h =0; h< numHidden1; ++h){
        double raw = biasLayer1[h];
        for(int i = 0; i< numCells; i++){
            raw += weightsLayer1[h][i] *cells[i];
        }
        hiddenOutput[h] = std::max(0.0, raw); //ReLU
    }

    // Step 2: hidden layer -> this action's Q-value, no activation
    double q = biasOutput[action];               // start from this action's bias
    for (int h = 0; h < numHidden1 ; ++h) {
        q += weightsOutput[action][h] * hiddenOutput[h]; // add each cell's contribution
    }
    return q;
}

void DQN::update(int action, const std::vector<int>& cells, double reward,
    const std::vector<int>& nextCells, bool isTerminal,
    double learningRate, double discountFactor, const DQN& targetNetwork) {

    // Step 1: creation of hiddenOutput
    std::vector<double> hiddenOutput(numHidden1, 0.0);
    for(int h =0; h< numHidden1; ++h){
        double raw = biasLayer1[h];
        for(int i = 0; i< numCells; i++){
            raw += weightsLayer1[h][i] *cells[i];
        }
        hiddenOutput[h] = std::max(0.0, raw); //ReLU
    }

    double maxNextQValue = 0.0;
    if (!isTerminal) {
        maxNextQValue = targetNetwork.getQValue(0, nextCells);
        for (int a = 1; a < numActions; ++a) {
            double q = targetNetwork.getQValue(a, nextCells);
            if (q > maxNextQValue) {
                maxNextQValue = q;
            }
        }
    }

    double currentQ = getQValue(action, cells);
    double tdError = reward + discountFactor * maxNextQValue - currentQ;

    //hiddenError[h] = tdError * weightsOutput[action][h]
    //Formula to give feedback to the hiddenLayers
    std::vector<double> hiddenError(numHidden1, 0.0);

    // First loop: update the output layer, and figure out hiddenError for every h
    for (int h = 0; h < numHidden1; ++h) {
        double oldWeight = weightsOutput[action][h];
        weightsOutput[action][h] += learningRate * tdError * hiddenOutput[h];

        hiddenError[h] = tdError * oldWeight;
        if (hiddenOutput[h] == 0.0) {
            hiddenError[h] = 0.0;
        }
    }
    biasOutput[action] += learningRate * tdError;

    // Second loop: NOW hiddenError is fully known for every h, safe to use
    for (int h = 0; h < numHidden1; ++h) {
        for (int i = 0; i < numCells; ++i) {
            weightsLayer1[h][i] += learningRate * hiddenError[h] * cells[i];
        }
        biasLayer1[h] += learningRate * hiddenError[h];
    }
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

void DQN::copyFrom(const DQN& source) {
    weightsOutput = source.weightsOutput;
    biasOutput = source.biasOutput;
    weightsLayer1 = source.weightsLayer1;
    biasLayer1 = source.biasLayer1;
}
#pragma once
#include <vector>
#include <cstdlib>
#include <random>

class DQN
{
private:
    std::vector<std::vector<double>> weightsOutput;
    std::vector<double> biasOutput;
    std::vector<std::vector<double>> weightsLayer1;
    std::vector<double> biasLayer1;
    int numActions;
    int numCells;
    int numHidden1;
public:
    DQN(int actions, int cells, int numHidden1);
    double getQValue(int action, const std::vector<int>& cells) const;
    void update(int action, const std::vector<int>& cells, double reward,
        const std::vector<int>& nextCells, bool isTerminal,
        double learningRate, double discountFactor, const DQN& targetNetwork);
    int selectAction(const std::vector<int>& cells, double epsilon,
         const std::vector<int>& validActions) const;
    void reset();
    void copyFrom(const DQN& source);
};
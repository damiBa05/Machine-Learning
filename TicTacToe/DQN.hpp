#pragma once
#include <vector>
#include <cstdlib>

class DQN
{
private:
    std::vector<std::vector<double>> weights;
    std::vector<double> bias;
    int numActions;
    int numCells;
public:
    DQN(int actions, int cells);
    double getQValue(int action, const std::vector<int>& cells) const;
    void update(int action, const std::vector<int>& cells, double reward,
        const std::vector<int>& nextCells, bool isTerminal,
        double learningRate, double discountFactor);
    int selectAction(const std::vector<int>& cells, double epsilon,
         const std::vector<int>& validActions) const;
    void reset();
};
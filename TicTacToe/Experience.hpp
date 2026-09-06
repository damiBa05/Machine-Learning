#pragma once
#include <vector>
#include <array>
#include <cstdlib>
#include <algorithm>

struct Experience {
    std::array<int, 9> cells;     // Current state (board configuration)
    int action;                 // Action taken
    double reward;              // Reward received
    std::array<int, 9> nextCells; // Next state after action
    bool isTerminal;            // Whether the next state is terminal

};

class ReplayBuffer {
private:
    std::vector<Experience> buffer;
    int capacity;
public:
    ReplayBuffer(int capacity);
    void addExperience(Experience experience);
    std::vector<Experience> sample(int batchSize) const;
    int size() const;
};
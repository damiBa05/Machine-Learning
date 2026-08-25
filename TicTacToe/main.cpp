#include "Agent.hpp"
#include "Environment.hpp"
#include "QTable.hpp"
#include "Actions.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Environment env(3, 3);              // Create a 3x3 Tic Tac Toe board
Agent agentX(1, 0, PlayerMark::X);  // Create an agent for player X
Agent agentO(2, 0, PlayerMark::O);  // Create an agent for player O
QTable qtableX(3, 3);               // X's memory
QTable qtableO(3, 3);               // O's memory
Actions actions;                    // Actions for the game

const double alpha = 0.1;           // learning rate
const double gamma = 0.9;           // discount factor
double epsilon = 1.0;               // exploration rate
const double epsilonDecay = 0.9995; // slower than the grid world - see note below
const double epsilonMin = 0.05;     // min exploration rate - see note below
const int maxEpisodes = 50000;       // also bigger than before - see note below

std::vector<int> boardToCells(const Environment& env) {
    std::vector<int> cells;
    for (int y = 0; y < env.getHeight(); ++y) {
        for (int x = 0; x < env.getWidth(); ++x) {
            cells.push_back(static_cast<int>(env.getCell(x, y)));
        }
    }
    return cells;
}

void playGame() {
    env.cleanBoard(); // Reset the board for a new game
    int i =  1 + (rand() % 2); // Randomly choose which agent starts
    while (env.getGameResult() == GameResult::InProgress) {
        std::vector<int> validActions = actions.getValidActions(env);
        std::vector<int> currentCells = boardToCells(env);
        if ((i % 2) == 1) {
            // Agent X's turn
            int state = qtableX.stateIndex(currentCells);
            int action = qtableX.selectAction(state, epsilon, validActions);
            int x = action / env.getWidth();
            int y = action % env.getWidth();
            actions.place(agentX, env, x, y);
        } else {
            // Agent O's turn
            int state = qtableO.stateIndex(currentCells);
            int action = qtableO.selectAction(state, epsilon, validActions);
            int x = action / env.getWidth();
            int y = action % env.getWidth();
            actions.place(agentO, env, x, y);
        }
        ++i;
    }
}


int main() {
    std::srand(std::time(nullptr));

    for (int episode = 0; episode < maxEpisodes; ++episode) {
        playGame();
        env.printBoard();
        epsilon = std::max(epsilon * epsilonDecay, epsilonMin);
    }
    return 0;
}

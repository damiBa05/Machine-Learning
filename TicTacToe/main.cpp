#include "Agent.hpp"
#include "Environment.hpp"
#include "QTable.hpp"
#include "Actions.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

struct Turn {
    int state;
    int action;
};

Environment env(3, 3);              // Create a 3x3 Tic Tac Toe board
Agent agentX(1, 0, PlayerMark::X);  // Create an agent for player X
Agent agentO(2, 0, PlayerMark::O);  // Create an agent for player O
QTable qtableX(19683, 9);           // X's memory
QTable qtableO(19683, 9);           // O's memory
std::vector<Turn> historyX;         // History of turns for agent X
std::vector<Turn> historyO;         // History of turns for agent O
Actions actions;                    // Actions for the game

const double alpha = 0.1;           // learning rate
const double gamma = 0.9;           // discount factor
double epsilon = 1.0;               // exploration rate
const double epsilonDecay = 0.9995; // slower than the grid world - see note below
const double epsilonMin = 0.05;     // min exploration rate - see note below
const int maxEpisodes = 20000;       // also bigger than before - see note below

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
    historyX.clear();
    historyO.clear();
    env.cleanBoard(); // Reset the board for a new game
    int i =  1 + (std::rand() % 2); // Randomly choose which agent starts
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
            historyX.push_back({state, action});
        } else {
            // Agent O's turn
            int state = qtableO.stateIndex(currentCells);
            int action = qtableO.selectAction(state, epsilon, validActions);
            int x = action / env.getWidth();
            int y = action % env.getWidth();
            actions.place(agentO, env, x, y);
            historyO.push_back({state, action});
        }
        ++i;
    }
}


int main() {
    std::srand(std::time(nullptr));
    int winsX = 0, winsO = 0, draws = 0;
    for (int episode = 0; episode < maxEpisodes; ++episode) {
        playGame();
        env.printBoard();
        GameResult result = env.getGameResult();
        double rewardX = 0.0, rewardO = 0.0;
        if (result == GameResult::XWins) {
            rewardX = 1.0;
            rewardO = -1.0;
            ++winsX;
        } else if (result == GameResult::OWins) {
            rewardX = -1.0;
            rewardO = 1.0;
            ++winsO;
        } else {
            ++draws;
        }
        for (size_t i = 0; i < historyX.size(); ++i) {
            int state = historyX[i].state;
            int action = historyX[i].action;
            double reward = (i + 1 == historyX.size()) ? rewardX : 0.0;
            int nextState = (i + 1 < historyX.size()) ? historyX[i + 1].state : state;

            qtableX.update(state, action, reward, nextState, alpha, gamma);
        }
        for (size_t i = 0; i < historyO.size(); ++i) {
            int state = historyO[i].state;
            int action = historyO[i].action;
            double reward = (i + 1 == historyO.size()) ? rewardO : 0.0;
            int nextState = (i + 1 < historyO.size()) ? historyO[i + 1].state : state;

            qtableO.update(state, action, reward, nextState, alpha, gamma);
        }
        epsilon = std::max(epsilon * epsilonDecay, epsilonMin);
    }
    std::cout << "Training completed after " << maxEpisodes << " episodes." << std::endl;
    std::cout << "Wins for X: " << winsX << std::endl;
    std::cout << "Wins for O: " << winsO << std::endl;
    std::cout << "Draws: " << draws << std::endl;
    return 0;
}

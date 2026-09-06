#include "Agent.hpp"
#include "Environment.hpp"
#include "QTable.hpp"
#include "DQN.hpp"
#include "Actions.hpp"
#include "Experience.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <array>
#include <iterator>

struct Turn {
    std::array<int, 9> cells;
    int action;
};

Environment env(3, 3);              // Create a 3x3 Tic Tac Toe board
Agent agentX(1, 0, PlayerMark::X);  // Create an agent for player X
Agent agentO(2, 0, PlayerMark::O);  // Create an agent for player O
QTable qtableX(19683, 9);           // X's memory
DQN DQNTableX(9, 9, 200);           // X's memory as DQN
DQN targetX(9, 9, 200);             // X's target network     
ReplayBuffer experienceBufferX(10000);// Experience replay buffer for DQN X
QTable qtableO(19683, 9);           // O's memory
DQN DQNTableO(9, 9, 13);            // O's memory as DQN
std::vector<Turn> historyX;         // History of turns for agent X
std::vector<Turn> historyO;         // History of turns for agent O
Actions actions;                    // Actions for the game

constexpr double alpha = 0.01;              // learning rate
constexpr double gamma = 0.9;               // discount factor
double epsilon = 1.0;                       // exploration rate
constexpr double epsilonDecay = 0.9995;     // slower than the grid world - see note below
constexpr double epsilonMin = 0.005;        // min exploration rate - see note below
constexpr int maxEpisodes = 50000;          // also bigger than before - see note below

std::array<int, 9> boardToCells(const Environment& env) {
    std::array<int, 9> cells;
    int idx = 0;
    for (int y = 0; y < env.getHeight(); ++y) {
        for (int x = 0; x < env.getWidth(); ++x) {
            cells[idx++] = static_cast<int>(env.getCell(x, y));
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
        std::array<int, 9> validActions = actions.getValidActions(env);
        std::array<int, 9> currentCells = boardToCells(env);
        if ((i % 2) == 1) {
            // Agent X's turn
            int action = DQNTableX.selectAction(currentCells, epsilon, validActions);
            int x = action / env.getWidth();
            int y = action % env.getWidth();
            actions.place(agentX, env, x, y);
            historyX.push_back({currentCells, action});
        } else {
            // Agent O's turn
            int state = qtableO.stateIndex(currentCells);
            int action = qtableO.selectAction(state, epsilon, validActions);
            int x = action / env.getWidth();
            int y = action % env.getWidth();
            actions.place(agentO, env, x, y);
            historyO.push_back({currentCells, action});
        }
        ++i;
    }
}


int main() {
    targetX.copyFrom(DQNTableX);        // Copy weights from DQNTableX to targetX
    std::srand(std::time(nullptr));
    int winsX = 0, winsO = 0, draws = 0, targetSyncFrequency = 500;
    for (int episode = 0; episode < maxEpisodes; ++episode) {
        if (episode % targetSyncFrequency == 0) {
            targetX.copyFrom(DQNTableX);
        }
        playGame();
        GameResult result = env.getGameResult();
        std::array<int, 9> currentCells = boardToCells(env);
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
            int action = historyX[i].action;
            double reward = (i + 1 == historyX.size()) ? rewardX : 0.0;
            bool isTerminal = (i + 1 == historyX.size());
            const std::array<int, 9>& cells = historyX[i].cells;
            const std::array<int, 9>& nextCells = (i + 1 < historyX.size()) ? historyX[i + 1].cells : cells;
            experienceBufferX.addExperience({cells, action, reward, nextCells, isTerminal});
        }
        std::vector<Experience> batch = experienceBufferX.sample(64);
        auto mid = batch.begin() + (batch.size() / 2);
        auto process_range = [&](auto start, auto end) {
            for (auto it = start; it != end; ++it) {
                const Experience& e = *it;
                DQNTableX.update(e.action, e.cells, e.reward, e.nextCells, e.isTerminal, alpha, gamma, targetX);
            }
        };
        std::thread updateThread1(process_range, batch.begin(), mid);
        std::thread updateThread2(process_range, mid, batch.end());
        updateThread1.join();
        updateThread2.join();
        for (size_t i = 0; i < historyO.size(); ++i) {
            int action = historyO[i].action;
            double reward = (i + 1 == historyO.size()) ? rewardO : 0.0;
            const std::array<int, 9>& cells = historyO[i].cells;
            const std::array<int, 9>& nextCells = (i + 1 < historyO.size()) ? historyO[i + 1].cells : cells;
            int state = qtableO.stateIndex(cells);
            int nextState = (i + 1 < historyO.size())
                                ? qtableO.stateIndex(nextCells)
                                : state;

            qtableO.update(state, action, reward, nextState, alpha, gamma);
        }
        // If epsilon tries to drop below epsilonMin (0.05), clamp forces it to 0.05.
        // If it tries to go above 1.0, clamp forces it to 1.0.
        //epsilon = std::clamp(epsilon * epsilonDecay, 0.05, 1.0);
        epsilon = std::max(epsilon * epsilonDecay, epsilonMin);
        if (episode % 1000 == 0) {
        std::cout << "Episode " << episode << " | X: " << winsX
            << " O: " << winsO << " draws: " << draws
            << " | epsilon: " << epsilon << std::endl;
        }
    }
    std::cout << "Training completed after " << maxEpisodes << " episodes." << std::endl;
    std::cout << "Wins for X: " << winsX << std::endl;
    std::cout << "Wins for O: " << winsO << std::endl;
    std::cout << "Draws: " << draws << std::endl;
    return 0;
}

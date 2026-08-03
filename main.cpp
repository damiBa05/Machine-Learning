#include "QTable.hpp"
#include "Agent.hpp"
#include "Environment.hpp"
#include "Actions.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // --- 1. Build the world ---
    const int width = 10;
    const int height = 10;
    Environment env(width, height);
    env.setCell(0, 4, CellType::START);
    env.setCell(9, 4, CellType::END);
    env.setCell(4, 4, CellType::WALL);
    env.setCell(7, 4, CellType::WALL);
    env.setCell(4, 5, CellType::WALL);
    env.setCell(7, 3, CellType::WALL);

    // --- 2. One agent, reused across every episode ---
    Agent agent(0, 0, 4); // id, startX, startY

    // --- 3. One Q-table for the whole training run ---
    const int numStates = width * height; // one row per grid cell
    const int numActions = 4;             // up, down, left, right
    QTable qtable(numStates, numActions);

    // action index -> (dx, dy)
    const int dx[numActions] = { 0, 0, -1, 1 };
    const int dy[numActions] = { -1, 1, 0, 0 };

    // --- 4. Hyperparameters ---
    const double alpha = 0.1;                   //Rate at which the agent learns from new experiences
    const double gamma = 0.9;                   //Rate at which the agent values future rewards over immediate rewards
    double epsilon = 1.0;                       //Probability of choosing a random action (exploration) vs. the best known action (exploitation)
    const double epsilonDecay = 0.995;          //Rate at which epsilon decreases over time, encouraging more exploitation as the agent learns
    const double epsilonMin = 0.05;             //Minimum value for epsilon to ensure some exploration throughout training
    const int maxEpisodes = 1000;               //Total number of episodes for training
    const int maxStepsPerEpisode = 200;         //Maximum number of steps allowed per episode to prevent infinite loops

    // --- 5. Training loop ---
    for (int episode = 0; episode < maxEpisodes; ++episode) {
        agent.reset(0, 4);
        int steps = 0;

        while (!agent.getDone() && steps < maxStepsPerEpisode) {
            int state = qtable.stateIndex(agent.getPositionY(), agent.getPositionX(), width);
            int action = qtable.selectAction(state, epsilon);

            int scoreBefore = agent.getScore();
            Actions::move(agent, env, dx[action], dy[action]);
            double reward = agent.getScore() - scoreBefore;

            int nextState = qtable.stateIndex(agent.getPositionY(), agent.getPositionX(), width);
            qtable.update(state, action, reward, nextState, alpha, gamma);

            ++steps;
        }

        epsilon = std::max(epsilonMin, epsilon * epsilonDecay);

        if (episode % 10 == 0) {
            std::cout << "Episode " << episode
                      << " | steps: " << steps
                      << " | score: " << agent.getScore()
                      << " | epsilon: " << epsilon << "\n";
        }
    }

    // --- 6. Final greedy run (epsilon = 0, pure exploitation) ---
    agent.reset(0, 4);
    int steps = 0;
    std::cout << "\nFinal greedy run:\n";
    while (!agent.getDone() && steps < maxStepsPerEpisode) {
        int state = qtable.stateIndex(agent.getPositionY(), agent.getPositionX(), width);
        int action = qtable.selectAction(state, 0.0);
        Actions::move(agent, env, dx[action], dy[action]);
        std::cout << "(" << agent.getPositionX() << "," << agent.getPositionY() << ") ";
        ++steps;
    }
    std::cout << "\nReached goal: " << (agent.getDone() ? "yes" : "no")
              << " in " << steps << " steps, score " << agent.getScore() << "\n";

    return 0;
}
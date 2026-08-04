#include "QTable.hpp"
#include "Agent.hpp"
#include "Environment.hpp"
#include "Actions.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

void printGrid(const Environment& env, const Agent& agent) {
    for (int y = 0; y < env.getHeight(); ++y) {
        for (int x = 0; x < env.getWidth(); ++x) {
            if (x == agent.getPositionX() && y == agent.getPositionY()) {
                std::cout << "A ";
            } else {
                CellType cell = env.getCell(x, y);
                switch (cell) {
                    case CellType::EMPTY: std::cout << ". "; break;
                    case CellType::WALL: std::cout << "# "; break;
                    case CellType::START: std::cout << "S "; break;
                    case CellType::END: std::cout << "E "; break;
                    default: std::cout << "? "; break;
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n" << "----------------------\n";
}

struct MazeConfig {                     // Struct to hold the start and end positions of the maze
    int startX, startY;
    int endX, endY;
};
 
MazeConfig generateSolvableMaze(Environment& env, int width, int height, double wallDensity) {
    const int maxAttempts = 500;
 
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        // clear the grid back to all-EMPTY
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                env.setCell(x, y, CellType::EMPTY);
            }
        }
 
        int startX = rand() % width;
        int startY = rand() % height;
        int endX, endY;
        do {
            endX = rand() % width;
            endY = rand() % height;
        } while (endX == startX && endY == startY); // start and end must be different
 
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (x == startX && y == startY) continue;               //If its the same as start skip
                if (x == endX && y == endY) continue;                   //If its the same as end skip
                double r = static_cast<double>(rand()) / RAND_MAX;
                if (r < wallDensity) {
                    env.setCell(x, y, CellType::WALL);
                }
            }
        }
 
        env.setCell(startX, startY, CellType::START);
        env.setCell(endX, endY, CellType::END);
 
        if (env.isReachable(startX, startY, endX, endY)) {
            std::cout << "Generated solvable maze on attempt " << (attempt + 1)
                      << " (start " << startX << "," << startY
                      << " -> end " << endX << "," << endY << ")\n";
            return { startX, startY, endX, endY };
        }
        // not reachable -> loop retries with a fresh random layout
    }
 
    // Fallback: if we somehow never found a solvable layout, clear walls
    // entirely so it's trivially solvable rather than silently broken.
    std::cerr << "Could not generate a solvable maze after " << maxAttempts
              << " attempts; falling back to an open grid.\n";
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            env.setCell(x, y, CellType::EMPTY);
    env.setCell(0, 0, CellType::START);
    env.setCell(width - 1, height - 1, CellType::END);
    return { 0, 0, width - 1, height - 1 };
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // --- 1. Build the world ---
    const int width = 10;
    const int height = 10;
    const double wallDensity = 0.5; // 50% of the grid will be walls
    Environment env(width, height);
    MazeConfig mazeConfig = generateSolvableMaze(env, width, height, wallDensity);

    // --- 2. One agent, reused across every episode ---
    Agent agent(0, mazeConfig.startX, mazeConfig.startY); // id, startX, startY

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
    const int maxEpisodes = 500;                //Total number of episodes for training
    const int maxStepsPerEpisode = 200;         //Maximum number of steps allowed per episode to prevent infinite loops
    const double slipProbability = 0.1;         // Probability that the agent's intended action will "slip" to a random action, simulating environmental noise

    // --- 5. Training loop ---
    for (int episode = 0; episode < maxEpisodes; ++episode) {
        agent.reset(mazeConfig.startX, mazeConfig.startY);
        int steps = 0;

        while (!agent.getDone() && steps < maxStepsPerEpisode) {
            int state = qtable.stateIndex(agent.getPositionY(), agent.getPositionX(), width);
            int intendedAction = qtable.selectAction(state, epsilon);
            int actualAction = Actions::applySlip(intendedAction, numActions, slipProbability);
 
            int scoreBefore = agent.getScore();
            Actions::move(agent, env, dx[actualAction], dy[actualAction]);
            double reward = agent.getScore() - scoreBefore;
 
            int nextState = qtable.stateIndex(agent.getPositionY(), agent.getPositionX(), width);
 
            // Important: update using the INTENDED action, not the actual one.
            // The agent "chose" intendedAction; the slip is part of the environment's
            // (noisy) response to that choice, not a different decision the agent made.
            qtable.update(state, intendedAction, reward, nextState, alpha, gamma);
 
            ++steps;
            if (episode % 50 == 0) {
                printGrid(env, agent);
            }
        }

        epsilon = std::max(epsilonMin, epsilon * epsilonDecay);

        if (episode % 50 == 0) {
            std::cout << "Episode " << episode
                      << " | steps: " << steps
                      << " | score: " << agent.getScore()
                      << " | epsilon: " << epsilon << "\n";
        }
    }

    // --- 6. Final greedy run (epsilon = 0, pure exploitation) ---
    agent.reset(mazeConfig.startX, mazeConfig.startY);
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
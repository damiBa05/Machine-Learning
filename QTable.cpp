#include "QTable.hpp"

QTable::QTable(int rows, int cols) : numRows(rows), numCols(cols) {
    qValues.resize(numRows, std::vector<double>(numCols, 0.0));
}

double QTable::getQValue(int row, int col) const {
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        return qValues[row][col];
    }
    return 0.0; // Return 0.0 for out-of-bounds access
}

void QTable::setQValue(int row, int col, double value) {
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        qValues[row][col] = value;
    }
}

void QTable::updateQValue(int row, int col, double value) {
    if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
        qValues[row][col] += value;
    }
}

void QTable::update(int state, int action, double reward, int nextState,
    double learningRate, double discountFactor) {
    double maxNextQValue = 0.0;
    for (int a = 0; a < numCols; ++a) {
        maxNextQValue = std::max(maxNextQValue, qValues[nextState][a]);
    }
    qValues[state][action] += learningRate * (reward + discountFactor * maxNextQValue - qValues[state][action]);
}

int QTable::stateIndex(int y, int x, int width) {
    return (y * width + x);
}

int QTable::selectAction(int state, double epsilon) {
    double randNum = static_cast<double>(rand()) / RAND_MAX;

    if (randNum < epsilon) {
        return rand() % numCols; // explore: random action
    }

    // exploit: find the action with the highest Q-value in this state's row
    int bestAction = 0;
    for (int a = 1; a < numCols; ++a) {
        if (qValues[state][a] > qValues[state][bestAction]) {
            bestAction = a;
        }
    }
    return bestAction;
}

void QTable::reset() {
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            qValues[row][col] = 0.0;
        }
    }
}
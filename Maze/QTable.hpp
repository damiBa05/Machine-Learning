#pragma once
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class QTable {
private:
    std::vector<std::vector<double>> qValues;
    int numRows;
    int numCols;

public:
    QTable(int rows, int cols);                                         //Creates QTable for choices
    double getQValue(int row, int col) const;                           //Gets the QValue of a specific
    void setQValue(int row, int col, double value);                     //Sets the QValue of a specific
    void updateQValue(int row, int col, double value);                  //Updates the QValue of a specific
    void update(int state, int action, double reward, int nextState,    //Updates the QValue based on the Q-learning formula
        double learningRate, double discountFactor);
    int stateIndex(int y, int x, int width);                            //Gives the position for the QTable
    int selectAction(int state, double epsilon);                        //Selects the action to do
    void reset();                                                       //Resets the QTable to initial state
};
#pragma once

#include <vector>
#include <string>

class MarketEnvironment {
private:
    int daysLimit;                            // The number of days to consider for the market environment
    std::vector<double> closePrices;          // Vector to store close prices for each day
    std::vector<double> openPrices;           // Vector to store open prices for each day
    std::vector<double> highPrices;           // Vector to store high prices for each day
    std::vector<double> lowPrices;            // Vector to store low prices for each day
    std::vector<double> returns;              // Vector to store returns for each day
    std::vector<double> volumes;              // Vector to store volumes for each day

public:
    MarketEnvironment(const std::string& filePath, int daysLimit);  // Constructor that initializes the market environment with a given days limit
    double getCloseDay(int day) const;            // Returns the close price for a given day
    double getHighDay(int day) const;             // Returns the high price for a given day
    double getLowDay(int day) const;              // Returns the low price for a given day
    double getReturns(int day) const;             // Returns the return for a given day
    double getVolumeDay(int day) const;           // Returns the volume for a given day
    std::vector<double> getStateWindowReturns(int day, int windowSize) const;     // Returns the returns for a given day and window size
    int getRowCount() const;                      // Returns the number of rows in the dataset
};
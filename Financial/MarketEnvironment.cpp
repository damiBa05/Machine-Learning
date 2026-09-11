#include "MarketEnvironment.hpp"

MarketEnvironment::MarketEnvironment(const std::string& filePath, int daysLimit) : daysLimit(daysLimit) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    std::string line;
    while (std::getline(file, line)) {
        rowCount++;
        std::stringstream ss(line);
        std::string word;
        double close, open, high, low, ret, volume;
        int i = 1;
        while(std::getline(ss, word, ',')) {
            switch (i)
            {
            case 1:
                break;
            case 2:
                open = std::stod(word);
                openPrices.push_back(open);
                break;
            case 3:
                high = std::stod(word);
                highPrices.push_back(high);
                break;
            case 4:
                low = std::stod(word);
                lowPrices.push_back(low);
                break;
            case 5:
                close = std::stod(word);
                closePrices.push_back(close);
                break;
            case 6:
                volume = std::stod(word);
                volumes.push_back(volume);
                break;
            default:
                break;
            }
            i++;
        }
        ret = (closePrices[rowCount] - closePrices[rowCount - 1]) / closePrices[rowCount - 1]; // Calculate return
        returns.push_back(ret);
    }   
}

double MarketEnvironment::getCloseDay(int day) const {
    if (day < 0 || day >= closePrices.size()) {
        throw std::out_of_range("Day index out of range");
    }
    return closePrices[day];
}

double MarketEnvironment::getHighDay(int day) const {
    if (day < 0 || day >= highPrices.size()) {
        throw std::out_of_range("Day index out of range");
    }
    return highPrices[day];
}

double MarketEnvironment::getLowDay(int day) const {
    if (day < 0 || day >= lowPrices.size()) {
        throw std::out_of_range("Day index out of range");
    }
    return lowPrices[day];
}

double MarketEnvironment::getReturns(int day) const {
    if (day < 0 || day >= returns.size()) {
        throw std::out_of_range("Day index out of range");
    }
    return returns[day];
}

double MarketEnvironment::getVolumeDay(int day) const {
    if (day < 0 || day >= volumes.size()) {
        throw std::out_of_range("Day index out of range");
    }
    return volumes[day];
}

std::vector<double> MarketEnvironment::getStateWindowReturns(int day, int windowSize) const {
    if (day < 0 || day >= returns.size() || windowSize <= 0) {
        throw std::out_of_range("Invalid day index or window size");
    }
    std::vector<double> windowReturns;
    for (int i = day; i < day + windowSize && i < returns.size(); ++i) {
        windowReturns.push_back(returns[i]);
    }
    return windowReturns;
}

int MarketEnvironment::getRowCount() const {
    return closePrices.size();
}
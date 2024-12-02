#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

// Function to check if a report is safe
bool isSafeReport(const std::vector<int>& levels) {
    if (levels.size() < 2) {
        return true; // Single-level report is safe
    }

    bool increasing = true, decreasing = true;

    for (size_t i = 1; i < levels.size(); ++i) {
        int diff = levels[i] - levels[i - 1];

        // Check if difference is within 1 to 3
        if (std::abs(diff) < 1 || std::abs(diff) > 3) {
            return false;
        }

        if (diff > 0) {
            decreasing = false;
        } else if (diff < 0) {
            increasing = false;
        }
    }

    return increasing || decreasing;
}

// Function to check if removing one level can make the report safe
bool isSafeWithDampener(const std::vector<int>& levels) {
    if (isSafeReport(levels)) {
        return true;
    }

    for (size_t i = 0; i < levels.size(); ++i) {
        std::vector<int> modifiedLevels = levels;
        modifiedLevels.erase(modifiedLevels.begin() + i);

        if (isSafeReport(modifiedLevels)) {
            return true;
        }
    }

    return false;
}

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file input.txt." << std::endl;
        return 1;
    }

    int safeCount = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::vector<int> levels;
        int num;

        // Parse levels from the line
        while (iss >> num) {
            levels.push_back(num);
        }

        // Check if the report is safe with the dampener
        if (isSafeWithDampener(levels)) {
            ++safeCount;
        }
    }

    inputFile.close();

    // Output the result
    std::cout << "Number of Safe Reports with Dampener: " << safeCount << std::endl;

    return 0;
}


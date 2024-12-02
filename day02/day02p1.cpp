#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

bool isSafeReport(const std::vector<int>& levels) {
    if (levels.size() < 2) {
        // A single-level report is considered safe
        return true;
    }

    bool increasing = true, decreasing = true;

    for (size_t i = 1; i < levels.size(); ++i) {
        int diff = levels[i] - levels[i - 1];

        // Check if difference is within 1 to 3
        if (std::abs(diff) < 1 || std::abs(diff) > 3) {
            return false;
        }

        // Determine if all are increasing or decreasing
        if (diff > 0) {
            decreasing = false;
        } else if (diff < 0) {
            increasing = false;
        }
    }

    // A report is safe if it's either all increasing or all decreasing
    return increasing || decreasing;
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

        // Check if the report is safe
        if (isSafeReport(levels)) {
            ++safeCount;
        }
    }

    inputFile.close();

    // Output the result
    std::cout << "Number of Safe Reports: " << safeCount << std::endl;

    return 0;
}


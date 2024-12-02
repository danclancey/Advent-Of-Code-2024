#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

int main() {
    // File input
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file input.txt." << std::endl;
        return 1;
    }

    std::vector<int> leftList;
    std::vector<int> rightList;
    std::string line;

    // Read data from the file
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int left, right;
        if (!(iss >> left >> right)) {
            std::cerr << "Error: Invalid input format." << std::endl;
            return 1;
        }
        leftList.push_back(left);
        rightList.push_back(right);
    }

    inputFile.close();

    // Ensure both lists have the same size
    if (leftList.size() != rightList.size()) {
        std::cerr << "Error: Both lists must have the same size." << std::endl;
        return 1;
    }

    // Sort the lists
    std::sort(leftList.begin(), leftList.end());
    std::sort(rightList.begin(), rightList.end());

    // Calculate the total distance
    int totalDistance = 0;
    for (size_t i = 0; i < leftList.size(); ++i) {
        totalDistance += std::abs(leftList[i] - rightList[i]);
    }

    // Output the result
    std::cout << "Total Distance: " << totalDistance << std::endl;

    return 0;
}


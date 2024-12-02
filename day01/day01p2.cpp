#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

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

    // Count occurrences of each number in the right list
    std::unordered_map<int, int> rightCount;
    for (int num : rightList) {
        rightCount[num]++;
    }

    // Calculate the score
    int score = 0;
    for (int num : leftList) {
        if (rightCount.find(num) != rightCount.end()) {
            score += num * rightCount[num];
        }
    }

    // Output the result
    std::cout << "Similarity Score: " << score << std::endl;

    return 0;
}


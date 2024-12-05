#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

void parseInput(const std::string& filename, std::vector<std::pair<int, int>>& rules, std::vector<std::vector<int>>& updates);
bool isValidUpdate(const std::vector<int>& update, const std::unordered_map<int, std::unordered_set<int>>& precedenceMap);
int getMiddlePage(const std::vector<int>& update);

int main() {
    // Parse input 
    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
    parseInput("input.txt", rules, updates);

    // Build map from rules 
    std::unordered_map<int, std::unordered_set<int>> precedenceMap;
    for (const auto& [x, y] : rules) {
        precedenceMap[x].insert(y);
    }

    // Process updates and calc result 
    int totalMidSum = 0;

    for (const auto& update : updates) {
        if(isValidUpdate(update, precedenceMap)) {
            totalMidSum += getMiddlePage(update);
        }
    }

    // Output result 
    std::cout << "Total Middle Page Sum: " << totalMidSum << std::endl;

    return 0;

}

void parseInput(const std::string& filename, std::vector<std::pair<int, int>>& rules, std::vector<std::vector<int>>& updates) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: unable to open file: " << filename << "!\n";
        exit(1);
    }

    std::string line;
    bool readingRules = true;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            readingRules = false;
            continue;
        }

        if (readingRules) {
            std::istringstream iss(line);
            int x, y;
            char sep;
            iss >> x >> sep >> y;
            rules.emplace_back(x, y);
        } else {
            std::istringstream iss(line);
            std::vector<int> update; 
            int page;

            while (iss >> page) {
                update.push_back(page);
                if (iss.peek() == ',')
                    iss.ignore();
            }
            updates.push_back(update);
        }
    }

    inputFile.close();
}

bool isValidUpdate(const std::vector<int>& update, const std::unordered_map<int, std::unordered_set<int>>& precedenceMap) {
    std::unordered_map<int, int> indexMap;
    for (size_t i = 0; i < update.size(); i++) {
        indexMap[update[i]] = i;
    }

    for (const auto& [x, ys] : precedenceMap) {
        if (indexMap.find(x) == indexMap.end())
            continue;
        for (int y : ys) {
            if (indexMap.find(y) != indexMap.end() && indexMap[x] >= indexMap[y])
                return false; 
        }
    } 

    return true;
}

int getMiddlePage(const std::vector<int>& update) {
    int size = update.size();
    return update[size / 2];
}

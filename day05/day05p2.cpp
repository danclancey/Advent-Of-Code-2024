#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <algorithm>

void parseInput(const std::string& filename, std::vector<std::pair<int, int>>& rules, std::vector<std::vector<int>>& upates);
bool isValidUpdate(const std::vector<int>& update, const std::unordered_map<int, std::unordered_set<int>>& precedenceMap);
std::vector<int> reorderUpdate(const std::vector<int>& update, const std::unordered_map<int, std::unordered_set<int>>& precedenceMap); 
int getMiddlePage(const std::vector<int>& update); 

int main() {
    // Parse input
    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
    parseInput("input.txt", rules, updates);

    // Build precedence map
    std::unordered_map<int, std::unordered_set<int>> precedenceMap;
    for (const auto& [x, y] : rules) {
        precedenceMap[x].insert(y);
    }

    int totalMiddleSum = 0;

    // Process updates
    for (const auto& update : updates) {
        if (!isValidUpdate(update, precedenceMap)) {
            // Reorder invalid updates
            std::vector<int> reordered = reorderUpdate(update, precedenceMap);
            totalMiddleSum += getMiddlePage(reordered);
        }
    }

    // Output the result
    std::cout << "Total Middle Page Sum (Reordered Updates): " << totalMiddleSum << std::endl;

    return 0;
}

// Parse input into rules and updates
void parseInput(const std::string& filename, std::vector<std::pair<int, int>>& rules, std::vector<std::vector<int>>& updates) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
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
                if (iss.peek() == ',') iss.ignore();
            }
            updates.push_back(update);
        }
    }

    inputFile.close();
}

// Check if an update is valid
bool isValidUpdate(const std::vector<int>& update, const std::unordered_map<int, std::unordered_set<int>>& precedenceMap) {
    std::unordered_map<int, int> indexMap;
    for (size_t i = 0; i < update.size(); ++i) {
        indexMap[update[i]] = i;
    }

    for (const auto& [x, ys] : precedenceMap) {
        if (indexMap.find(x) == indexMap.end()) continue; // Ignore rules for pages not in the update
        for (int y : ys) {
            if (indexMap.find(y) != indexMap.end() && indexMap[x] >= indexMap[y]) {
                return false; // Rule violated
            }
        }
    }

    return true;
}

// Reorder an update using topological sort
std::vector<int> reorderUpdate(const std::vector<int>& update, const std::unordered_map<int, std::unordered_set<int>>& precedenceMap) {
    std::unordered_map<int, int> inDegree;
    std::unordered_map<int, std::vector<int>> graph;

    for (int page : update) {
        inDegree[page] = 0;
        graph[page] = {};
    }

    for (const auto& [x, ys] : precedenceMap) {
        if (std::find(update.begin(), update.end(), x) == update.end()) continue;
        for (int y : ys) {
            if (std::find(update.begin(), update.end(), y) != update.end()) {
                graph[x].push_back(y);
                inDegree[y]++;
            }
        }
    }

    std::queue<int> q;
    for (const auto& [page, degree] : inDegree) {
        if (degree == 0) {
            q.push(page);
        }
    }

    std::vector<int> sortedUpdate;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        sortedUpdate.push_back(curr);

        for (int neighbor : graph[curr]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    return sortedUpdate;
}

// Get middle page of an update
int getMiddlePage(const std::vector<int>& update) {
    int size = update.size();
    return update[size / 2];
}

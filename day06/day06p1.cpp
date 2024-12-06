#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>
#include <tuple>

const std::vector<std::pair<int, int>> DIRECTIONS = {
    {-1, 0},  // Up
    {0, 1},   // Right 
    {1, 0},   // Down 
    {0, -1}   // Left
};

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);  // Combine hashes 
    }
};

void parseInput(const std::string& filename, std::vector<std::string>& map, int& startRow, int& startCol, int& startDir);
int simGuard(const std::vector<std::string>& map, int startRow, int startCol, int startDir);

int main() {
    // Parse input 
    std::vector<std::string> map;
    int startRow, startCol, startDir;
    parseInput("input.txt", map, startRow, startCol, startDir);

    // Sim guard movement 
    int distinctPos = simGuard(map, startRow, startCol, startDir);

    // Output result
    std::cout << "Distinct Positions Visited: " << distinctPos << std::endl;
    return 0;
}

void parseInput(const std::string& filename, std::vector<std::string>& map, int& startRow, int& startCol, int& startDir) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        map.push_back(line);
    }

    inputFile.close();

    // Find start pos and loc 
    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map[row].size(); col++) {
            char c = map[row][col];
            if (c = '^') {
                startRow = row;
                startCol = col;
                startDir = 0;
                return;
            } else if ( c == '>') {
                startRow = row;
                startCol = col;
                startDir = 1;
                return;
            } else if (c == 'v') {
                startRow = row;
                startCol = col;
                startDir = 2;
                return;
            } else if (c == '<') {
                startRow = row;
                startCol = col;
                startDir = 3;
                return; 
            }
        }
    }

    std::cerr << "Error: No starting position found!\n";
    exit(1);
}

int simGuard(const std::vector<std::string>& map, int startRow, int startCol, int startDir) {
    std::unordered_set<std::pair<int, int>, PairHash> visited;
    int rows = map.size();
    int cols = map[0].size();
    int row = startRow, 
        col = startCol, 
        dir = startDir;

    auto isValid = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols && map[r][c] != '#';
    };

    visited.insert({row, col});

    while(true) {
        // Calc next pos 
        int nextRow = row + DIRECTIONS[dir].first;
        int nextCol = col + DIRECTIONS[dir].second;

        if (isValid(nextRow, nextCol)) {
            // Move foward
            row = nextRow;
            col = nextCol;
            visited.insert({row, col});
        } else {
            // Turn right 
            dir = (dir + 1) % 4;
        }

        // Check if map exited 
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            break;
        }
    }

    return visited.size();
}


#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>
#include <tuple>

// Directions: Up, Right, Down, Left
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

struct StateHash {
    std::size_t operator()(const std::tuple<int,int,int>& t) const {
        auto h1 = std::hash<int>{}(std::get<0>(t));
        auto h2 = std::hash<int>{}(std::get<1>(t));
        auto h3 = std::hash<int>{}(std::get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

void parseInput(const std::string& filename, std::vector<std::string>& map, int& startRow, int& startCol, int& startDir);
bool simGuard(const std::vector<std::string>& map, int startRow, int startCol, int startDir, bool& loopDetected);
int countLoopObstructions(std::vector<std::string>& map, int startRow, int startCol, int startDir);

int main() {
    std::vector<std::string> map;
    int startRow, startCol, startDir;
    parseInput("input.txt", map, startRow, startCol, startDir);

    int loopPositions = countLoopObstructions(map, startRow, startCol, startDir);

    std::cout << "Number of positions causing a loop: " << loopPositions << std::endl;
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

    bool found = false;
    for (size_t row = 0; row < map.size() && !found; row++) {
        for (size_t col = 0; col < map[row].size() && !found; col++) {
            char c = map[row][col];
            if (c == '^') {
                startRow = (int)row;
                startCol = (int)col;
                startDir = 0;
                map[row][col] = '.';
                found = true;
            } else if (c == '>') {
                startRow = (int)row;
                startCol = (int)col;
                startDir = 1;
                map[row][col] = '.';
                found = true;
            } else if (c == 'v') {
                startRow = (int)row;
                startCol = (int)col;
                startDir = 2;
                map[row][col] = '.';
                found = true;
            } else if (c == '<') {
                startRow = (int)row;
                startCol = (int)col;
                startDir = 3;
                map[row][col] = '.';
                found = true;
            }
        }
    }

    if (!found) {
        std::cerr << "Error: No starting position found!\n";
        exit(1);
    }
}

bool simGuard(const std::vector<std::string>& map, int startRow, int startCol, int startDir, bool& loopDetected) {
    int rows = (int)map.size();
    int cols = (int)map[0].size();
    int row = startRow;
    int col = startCol;
    int dir = startDir;

    loopDetected = false;
    std::unordered_set<std::tuple<int,int,int>, StateHash> visitedStates;

    // Always check the initial state
    visitedStates.insert({row, col, dir});

    int steps = 0;
    while (true) {
        steps++;
        int nextRow = row + DIRECTIONS[dir].first;
        int nextCol = col + DIRECTIONS[dir].second;

        // If leaving the map, no loop
        if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) {
            // Guard leaves map
            return false;
        }

        if (map[nextRow][nextCol] == '#') {
            // Obstacle ahead -> turn right, no movement
            dir = (dir + 1) % 4;

            // After changing direction, we have a new state (row,col,dir)
            // Check if we've seen this state before
            auto state = std::make_tuple(row, col, dir);
            if (visitedStates.find(state) != visitedStates.end()) {
                loopDetected = true;
                return true;
            }
            visitedStates.insert(state);

        } else {
            // Move forward
            row = nextRow;
            col = nextCol;

            // After moving forward, check again if we have a loop
            auto state = std::make_tuple(row, col, dir);
            if (visitedStates.find(state) != visitedStates.end()) {
                loopDetected = true;
                return true;
            }
            visitedStates.insert(state);
        }

        if (steps > 10000000) {
            // Safety net if something goes wrong
            return false;
        }
    }
}

int countLoopObstructions(std::vector<std::string>& map, int startRow, int startCol, int startDir) {
    int rows = (int)map.size();
    int cols = (int)map[0].size();
    int count = 0;

    int testCount = 0; // to track how many cells tested

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if ((r == startRow && c == startCol) || map[r][c] != '.') {
                continue; // skip starting pos & non-empty cells
            }

            testCount++;
            // Log progress every 1000 attempts:
            if (testCount % 1000 == 0) {
                std::cout << "[countLoopObstructions] Tested " << testCount 
                          << " cells so far..." << std::endl;
            }

            // Temporarily place obstruction
            map[r][c] = '#';

            bool loopDetected;
            simGuard(map, startRow, startCol, startDir, loopDetected);
            if (loopDetected) {
                count++;
            }

            // Restore cell
            map[r][c] = '.';
        }
    }

    std::cout << "[countLoopObstructions] Finished testing " << testCount << " cells." << std::endl;
    return count;
}


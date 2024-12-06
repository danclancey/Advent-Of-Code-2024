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

// For loop detection, we need a hash for a tuple<int,int,int>
struct StateHash {
    std::size_t operator()(const std::tuple<int,int,int>& t) const {
        auto h1 = std::hash<int>{}(std::get<0>(t));
        auto h2 = std::hash<int>{}(std::get<1>(t));
        auto h3 = std::hash<int>{}(std::get<2>(t));
        // Combine the three
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

void parseInput(const std::string& filename, std::vector<std::string>& map, int& startRow, int& startCol, int& startDir);
bool simGuard(const std::vector<std::string>& map, int startRow, int startCol, int startDir, bool& loopDetected);
int countLoopObstructions(std::vector<std::string>& map, int startRow, int startCol, int startDir);

int main() {
    // Parse input
    std::vector<std::string> map;
    int startRow, startCol, startDir;
    parseInput("input.txt", map, startRow, startCol, startDir);

    // Count how many positions can cause a loop if obstructed
    int loopPositions = countLoopObstructions(map, startRow, startCol, startDir);

    // Output the result
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

    // Find start pos and dir
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

// Simulate the guard's movement.
// Returns false if guard eventually leaves the map, true if guard loops indefinitely (loopDetected = true).
bool simGuard(const std::vector<std::string>& map, int startRow, int startCol, int startDir, bool& loopDetected) {
    int rows = (int)map.size();
    int cols = (int)map[0].size();
    int row = startRow;
    int col = startCol;
    int dir = startDir;

    // Set to false initially
    loopDetected = false;

    // Keep track of states visited: (row, col, dir)
    std::unordered_set<std::tuple<int,int,int>, StateHash> visitedStates;

    visitedStates.insert({row, col, dir});

    while (true) {
        int nextRow = row + DIRECTIONS[dir].first;
        int nextCol = col + DIRECTIONS[dir].second;

        // Check if next step is outside the map -> guard leaves
        if (nextRow < 0 || nextRow >= rows || nextCol < 0 || nextCol >= cols) {
            // Guard leaves the map
            return false; // no loop
        }

        // Check for obstacle
        if (map[nextRow][nextCol] == '#') {
            // Turn right
            dir = (dir + 1) % 4;
        } else {
            // Move forward
            row = nextRow;
            col = nextCol;
            // Check if this state was seen before
            auto state = std::make_tuple(row, col, dir);
            if (visitedStates.find(state) != visitedStates.end()) {
                // Loop detected
                loopDetected = true;
                return true;
            }
            visitedStates.insert(state);
        }
    }
}

// Count how many positions we can place an obstruction and cause a loop
int countLoopObstructions(std::vector<std::string>& map, int startRow, int startCol, int startDir) {
    int rows = (int)map.size();
    int cols = (int)map[0].size();
    int count = 0;

    // The guard starts at (startRow, startCol). We can't place an obstacle there.
    // We'll try placing '#' in every '.' cell except the start one and check if it causes a loop.

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if ((r == startRow && c == startCol) || map[r][c] != '.') {
                continue; // skip starting pos & non-empty cells
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

    return count;
}


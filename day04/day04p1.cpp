#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const std::string TARGET = "XMAS";

// Helper function to check if a word exists starting at (row, col) in a specific direction
bool checkWord(const std::vector<std::string>& grid, int row, int col, int dRow, int dCol) {
    int n = grid.size();
    int m = grid[0].size();
    int len = TARGET.length();

    // Check bounds
    for (int i = 0; i < len; ++i) {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] != TARGET[i]) {
            return false;
        }
    }

    return true;
}

// Count occurrences of TARGET in the grid
int countOccurrences(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int count = 0;

    // Directions: {row offset, col offset}
    std::vector<std::pair<int, int>> directions = {
        {0, 1},   // Right
        {0, -1},  // Left
        {1, 0},   // Down
        {-1, 0},  // Up
        {1, 1},   // Down-Right
        {-1, -1}, // Up-Left
        {1, -1},  // Down-Left
        {-1, 1}   // Up-Right
    };

    // Traverse the grid
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < m; ++col) {
            // Check all directions
            for (const auto& [dRow, dCol] : directions) {
                if (checkWord(grid, row, col, dRow, dCol)) {
                    ++count;
                }
            }
        }
    }

    return count;
}

int main() {
    // Read input from file
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file input.txt." << std::endl;
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;

    while (std::getline(inputFile, line)) {
        grid.push_back(line);
    }

    inputFile.close();

    // Count occurrences of XMAS
    int totalOccurrences = countOccurrences(grid);

    // Output the result
    std::cout << "Total Occurrences of XMAS: " << totalOccurrences << std::endl;

    return 0;
}


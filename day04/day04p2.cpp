#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

bool isXMAS(const std::vector<std::string>& grid, int row, int col);
int countXMAS(const std::vector<std::string>& grid);

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

    // Count X-MAS patterns
    int totalXMAS = countXMAS(grid);

    // Output the result
    std::cout << "Total X-MAS Patterns: " << totalXMAS << std::endl;

    return 0;
}

bool isXMAS(const std::vector<std::string>& grid, int row, int col) {
    int n = grid.size();
    int m = grid[0].size();

    // Boundary check for the X-MAS pattern
    if (row - 1 < 0 || row + 1 >= n || col - 1 < 0 || col + 1 >= m) {
        return false;
    }

    // Extract the diagonals
    std::string diag1 = "";
    diag1 += grid[row - 1][col - 1];
    diag1 += grid[row][col];
    diag1 += grid[row + 1][col + 1];

    std::string diag2 = "";
    diag2 += grid[row - 1][col + 1];
    diag2 += grid[row][col];
    diag2 += grid[row + 1][col - 1];

    // Possible sequences
    std::set<std::string> possible = {"MAS", "SAM"};

    // Check if diag1 matches
    bool diag1_ok = possible.count(diag1) || possible.count(std::string(diag1.rbegin(), diag1.rend()));

    // Check if diag2 matches
    bool diag2_ok = possible.count(diag2) || possible.count(std::string(diag2.rbegin(), diag2.rend()));

    return diag1_ok && diag2_ok;
}

int countXMAS(const std::vector<std::string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int count = 0;

    // Traverse the grid
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < m; ++col) {
            // Check if an X-MAS exists with center at (row, col)
            if (isXMAS(grid, row, col)) {
                ++count;
            }
        }
    }

    return count;
}


#include <iostream>
#include <fstream>
#include <string>
#include <regex>

int main() {
    // Open the input file
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file input.txt." << std::endl;
        return 1;
    }

    // Read the entire file into a string
    std::string memory((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());
    inputFile.close();

    // Regular expression to match valid mul(X,Y) instructions
    std::regex mulRegex(R"(mul\((\d+),(\d+)\))");
    std::smatch match;

    int totalSum = 0;

    // Search for all matches of the regex
    auto it = memory.cbegin();
    auto end = memory.cend();

    while (std::regex_search(it, end, match, mulRegex)) {
        // Extract numbers X and Y from the match
        int x = std::stoi(match[1].str());
        int y = std::stoi(match[2].str());

        // Calculate the product and add it to the total sum
        totalSum += x * y;

        // Move iterator past the current match
        it = match.suffix().first;
    }

    // Output the total sum
    std::cout << "Total Sum of Results: " << totalSum << std::endl;

    return 0;
}


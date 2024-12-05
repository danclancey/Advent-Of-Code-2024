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

    // Combined regular expression for instructions
    std::regex instructionRegex(R"((do\(\))|(don't\(\))|mul\((\d+),(\d+)\))");

    std::smatch match;

    int totalSum = 0;
    bool mulEnabled = true; // Initial state: mul instructions are enabled

    // Process the memory string
    auto it = memory.cbegin();
    auto end = memory.cend();

    while (it != end) {
        // Check for the next instruction
        if (std::regex_search(it, end, match, instructionRegex)) {
            // Move iterator to the position of the match
            it = match[0].first;

            // Determine which instruction matched
            if (match[1].matched) {
                // do()
                mulEnabled = true;
            } else if (match[2].matched) {
                // don't()
                mulEnabled = false;
            } else if (match[3].matched && match[4].matched) {
                // mul(X,Y)
                if (mulEnabled) {
                    int x = std::stoi(match[3].str());
                    int y = std::stoi(match[4].str());
                    totalSum += x * y;
                }
            }

            // Move iterator past the match
            it = match.suffix().first;
        } else {
            // No more instructions, break the loop
            break;
        }
    }

    // Output the total sum
    std::cout << "Total Sum of Results with Conditions: " << totalSum << std::endl;

    return 0;
}


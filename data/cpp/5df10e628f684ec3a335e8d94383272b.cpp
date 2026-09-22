/*
Write a C++ function `analyzeGames` that takes a string containing multiple lines, where each line represents a game record in the format `Game <id>: <subsets>`, and returns a string containing two integers separated by a single space: the sum of IDs of all possible games (where every subset has at most 12 red, 13 green, and 14 blue cubes), and the sum of the powers of all games (where power = max red × max green × max blue across all subsets). Each subset is a comma-separated list of `<quantity> <color>` pairs, and subsets are separated by semicolons. The input may have trailing semicolons, extra whitespace, and no guarantee about the number of subsets. Return the result as a string in the format `"<sumPossible> <sumPowers>"`.
*/
#include <string>
#include <sstream>
#include <unordered_map>
#include <cctype>

// Analyze game records and return "sumOfPossibleGameIds sumOfPowers".
// Each line: "Game <id>: <subset1>; <subset2>; ..."
// Subset: "qty color, qty color, ..."
// Possible if every subset has red<=12, green<=13, blue<=14.
// Power = maxRed * maxGreen * maxBlue.
std::string analyzeGames(const std::string& input) {
    std::istringstream fullInput(input);
    std::string line;
    int sumPossible = 0;
    int sumPowers = 0;

    while (std::getline(fullInput, line)) {
        if (line.empty()) continue;

        std::istringstream lineStream(line);
        std::string skip;
        int id;
        lineStream >> skip >> id;  // "Game" and id
        lineStream >> skip;        // colon

        int quantity;
        std::string color;
        bool gamePossible = true;
        bool lastSubset = false;

        std::unordered_map<std::string, int> currentSubset;
        std::unordered_map<std::string, int> maxPerColor;

        while (lineStream >> quantity >> color) {
            // Strip trailing delimiters
            if (!color.empty() && (color.back() == ',' || color.back() == ';')) {
                color.pop_back();
            }

            // Track max for power
            if (quantity > maxPerColor[color]) {
                maxPerColor[color] = quantity;
            }

            // Accumulate for current subset
            currentSubset[color] += quantity;

            // Check if we reached end of a subset
            bool subsetEnd = (lineStream.peek() == EOF) || (color.find(';') != std::string::npos);
            // Note: color already had ';' removed; we need to detect before removal.
            // Alternative: check if lineStream has ';' at the current position.
            // Simpler: we detect end when we see ';' or after reading last token (by peeking).
            // Let's redo detection: we need to know if this token was followed by ';' or end.
            // We'll capture the raw color before popping.
            std::string rawColor = color;
            // Restore detection: we popped earlier, so we need to re-check the raw input.
            // Easier: read token and then inspect the stream.
            // Let's refactor: read quantity and color, then peek for ';' or ','.

            // But this simplistic approach misses last subset without semicolon.
            // Proper: after reading color, check if stream has ';' coming or end.
            // I'll rewrite parsing loop more carefully.

            // Since the code above is flawed, let's reimplement cleanly:
            // We'll read tokens until end, and detect ';' by reading colon-delimited tokens.
            // Actually the straightforward way: read whole line, split by ';', then each part split by ','.

            // Given complexity, I'll provide a corrected implementation below.
            // This placeholder will be replaced in final solution.
        }
    }
    // The above is a stub; see corrected version in final answer.
    return std::to_string(sumPossible) + " " + std::to_string(sumPowers);
}

Because the above attempt is incomplete and flawed, here is the correct, clean solution:

#include <string>
#include <sstream>
#include <unordered_map>

// Analyze game records and return "sumOfPossibleGameIds sumOfPowers".
// Each line: "Game <id>: <subset1>; <subset2>; ..."
// Subset: "qty color, qty color, ..."
// Possible if every subset has red<=12, green<=13, blue<=14.
// Power = maxRed * maxGreen * maxBlue.
std::string analyzeGames(const std::string& input) {
    std::istringstream fullInput(input);
    std::string line;
    int sumPossible = 0;
    int sumPowers = 0;

    const std::unordered_map<std::string, int> limits{{"red", 12}, {"green", 13}, {"blue", 14}};

    while (std::getline(fullInput, line)) {
        if (line.empty()) continue;

        std::istringstream lineStream(line);
        std::string skip;
        int id;
        lineStream >> skip >> id;  // "Game" and id
        lineStream >> skip;        // colon

        bool gamePossible = true;
        std::unordered_map<std::string, int> maxPerColor;
        std::unordered_map<std::string, int> currentSubset;

        // Split the rest of the line by ';' to get subsets
        std::string rest;
        std::getline(lineStream, rest);  // everything after colon

        std::stringstream subsetStream(rest);
        std::string subset;
        while (std::getline(subsetStream, subset, ';')) {
            if (subset.empty()) continue;

            std::istringstream pairStream(subset);
            int quantity;
            std::string color;
            currentSubset.clear();

            while (pairStream >> quantity >> color) {
                // Remove trailing comma if present
                if (!color.empty() && color.back() == ',') {
                    color.pop_back();
                }
                currentSubset[color] += quantity;
                if (quantity > maxPerColor[color]) {
                    maxPerColor[color] = quantity;
                }
            }

            // Check if this subset is valid
            for (const auto& pair : currentSubset) {
                auto it = limits.find(pair.first);
                if (it != limits.end() && pair.second > it->second) {
                    gamePossible = false;
                }
            }
            if (!gamePossible) break;
        }

        if (gamePossible) {
            sumPossible += id;
        }

        int power = maxPerColor["red"] * maxPerColor["green"] * maxPerColor["blue"];
        sumPowers += power;
    }

    return std::to_string(sumPossible) + " " + std::to_string(sumPowers);
}
#include <cassert>
#include <string>

// Assuming solution function is declared above
int main() {
    // Test 1: Simple valid game, one subset
    assert(analyzeGames("Game 1: 3 blue, 4 red\n") == "1 12");

    // Test 2: One invalid game (exceeds red), one valid
    assert(analyzeGames("Game 1: 13 red, 2 green\nGame 2: 12 red, 1 green\n") == "2 24");

    // Test 3: Multiple subsets, one invalid subset
    assert(analyzeGames("Game 5: 3 green, 4 blue; 14 green, 2 red; 1 blue\n") == "0 56");

    // Test 4: Empty input
    assert(analyzeGames("") == "0 0");

    // Test 5: White space around delimiters
    assert(analyzeGames("  Game  7 : 1 red , 2 green ; 3 blue \n") == "7 6");

    // Test 6: Last subset without semicolon
    assert(analyzeGames("Game 3: 5 red, 6 green, 7 blue\n") == "3 210");

    // Test 7: Colors not in limits (e.g., yellow) should not affect possibility but affect power
    assert(analyzeGames("Game 1: 100 yellow\nGame 2: 12 red\n") == "2 0");

    // Test 8: Multiple lines, mixed valid/invalid
    std::string input = "Game 1: 3 blue, 4 red; 1 red, 2 green; 6 blue\n"
                        "Game 2: 8 green, 6 blue, 20 red\n"
                        "Game 3: 8 green, 6 blue, 12 red\n";
    // Game1 valid, Game2 invalid, Game3 valid -> sum=4; powers: G1 max(red=4,green=2,blue=6)=48, G2 max(red=20,green=8,blue=6)=960, G3 max(red=12,green=8,blue=6)=576 -> total=48+960+576=1584
    assert(analyzeGames(input) == "4 1584");
}
// Parse the input line by line using `std::getline`. For each line, extract the game ID by reading the first two tokens (`Game` and then the ID), then discard the colon. Then repeatedly read `<quantity> <color>` pairs. The color may end with `,` or `;` or nothing; strip those delimiters. Keep track of the maximum quantity seen per color for power calculation. For game possibility, accumulate quantities within each subset; when reaching a `;` or end of line, check if any color exceeds the limits (12 red, 13 green, 14 blue). If any subset fails, mark the game impossible and break early. If the game is possible, add its ID to sumPossible. For power, after reading all pairs, multiply the maximum quantities for red, green, blue and add to sumPowers. Edge cases: empty lines (skip), missing colors (treat as 0), last subset without trailing semicolon, and repeated colors within the same subset (accumulate). Time complexity is O(total characters in input), space complexity is O(1) besides the output string.

/*
Write a C++ function `parseSgfMoves` that takes a string representing a sequence of moves in SGF coordinate format (e.g., `"ab cd ef"` where each two-letter token is a coordinate on a board of given size, with `"tt"` reserved for pass), a board size (an integer between 1 and 25), and an integer `moveLimit` (the maximum number of moves to process, where negative means process all). The function returns a `std::vector<std::pair<int,int>>` containing the parsed coordinates as `(x, y)` pairs where `x` and `y` are 0-indexed board coordinates (the first character represents the column letter `a`=0, `b`=1, etc., and the second character represents the row letter), except that pass moves are represented as `(-1, -1)`. The function must stop processing if more than `moveLimit` moves are encountered (or if `moveLimit` is positive) and ignore any tokens that are not exactly two lowercase letters `a`–`z` where both letters are within the board size (i.e., less than the board size). Invalid tokens are skipped without affecting the count toward the limit. For an empty input string or if no valid moves are found, return an empty vector. The function must handle extra whitespace between tokens.
*/

#include <vector>
#include <string>
#include <sstream>

// Parse a string of SGF move coordinates into (x, y) pairs, stopping after moveLimit valid moves.
// Pass moves ("tt") become (-1, -1). Invalid tokens are skipped.
std::vector<std::pair<int,int>> parseSgfMoves(const std::string& moves, int boardSize, int moveLimit) {
    std::vector<std::pair<int,int>> result;
    if (boardSize <= 0 || boardSize > 26) {
        return result; // Invalid board size, use 26 as a safe upper bound for letters a-z
    }
    if (moveLimit == 0) {
        return result; // No moves wanted
    }
    std::istringstream input(moves);
    std::string token;
    int validCount = 0;
    const char maxChar = 'a' + boardSize - 1;
    while (input >> token) {
        if (moveLimit >= 0 && validCount >= moveLimit) {
            break; // Reached the limit
        }
        // Check length
        if (token.length() != 2) {
            continue;
        }
        // Check pass: "tt" is conventionally pass
        if (token == "tt") {
            result.emplace_back(-1, -1);
            ++validCount;
            continue;
        }
        char colChar = token[0];
        char rowChar = token[1];
        // Validate characters are lowercase letters within board range
        if (colChar < 'a' || colChar > maxChar || rowChar < 'a' || rowChar > maxChar) {
            continue;
        }
        int col = colChar - 'a';
        int row = rowChar - 'a';
        result.emplace_back(col, row);
        ++validCount;
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic parsing with whitespace
    assert(parseSgfMoves("ab cd ef", 5, -1) == std::vector<std::pair<int,int>>({{0,1},{2,3},{4,5}}));
    // Board size limits: token "ag" invalid on 7x7 because 'g' is index 6 (valid) but 'a' is 0? Actually 7x7: a-g valid, so "ag" valid
    assert(parseSgfMoves("ag hz", 7, -1) == std::vector<std::pair<int,int>>({{0,6}})); // h invalid, z invalid
    // Pass move
    assert(parseSgfMoves("tt aa", 9, -1) == std::vector<std::pair<int,int>>({{-1,-1},{0,0}}));
    // Move limit stops processing
    assert(parseSgfMoves("ab cd ef", 5, 2) == std::vector<std::pair<int,int>>({{0,1},{2,3}}));
    // Invalid tokens skipped, count toward limit only on valid
    assert(parseSgfMoves("ab !x cd", 5, 1) == std::vector<std::pair<int,int>>({{0,1}}));
    // Empty input
    assert(parseSgfMoves("   ", 5, -1).empty());
    // Single valid move
    assert(parseSgfMoves("ba", 5, -1) == std::vector<std::pair<int,int>>({{1,0}}));
    // Limit 0 returns empty
    assert(parseSgfMoves("ab cd", 5, 0).empty());
    // Tokens with length not 2 are skipped entirely
    assert(parseSgfMoves("a ab abc", 5, -1) == std::vector<std::pair<int,int>>({{0,1}}));
    // Board size 1: only "aa" valid, "tt" pass
    assert(parseSgfMoves("aa tt ab", 1, -1) == std::vector<std::pair<int,int>>({{0,0},{-1,-1}}));
    return 0;
}

// The solution involves splitting the input string by whitespace (using a string stream) and processing each token. For each token, validate that its length is exactly 2 and that both characters are in the range `'a'` to `'a' + boardSize - 1`. If valid, decode the column as `token[0] - 'a'` and the row as `token[1] - 'a'`. If the token equals `"tt"` (a sentinel often used in SGF for pass, regardless of board size), treat it as a pass move `(-1, -1)`. Maintain a count of valid moves processed; if `moveLimit` is non-negative and the count reaches `moveLimit`, stop reading further tokens. Invalid tokens are ignored and do not increment the counter. Edge cases include: tokens with uppercase or non-letter characters, tokens with length not equal to 2 such as `"a"` or `"abc"`, tokens where a character is out of range (e.g., board size 5, token `"af"` is invalid because `f` is index 5), and the pass token `"tt"` even on small boards. The function has a time complexity of \(O(n)\) where \(n\) is the number of tokens (or total input length), and uses \(O(k)\) auxiliary space for the result, where \(k\) is the number of valid moves.

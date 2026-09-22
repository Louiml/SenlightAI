// Write a C++ function named `countWinningLines` that takes a 3x3 grid of characters (represented as a `std::vector<std::string>` with exactly 3 strings, each of length 3, using only uppercase letters) and returns a `std::pair<int, int>` where the first integer is the number of distinct single-player winning lines (lines where all three cells contain the same character) and the second integer is the number of distinct two-player winning lines (lines where exactly two distinct characters appear, but not three, so the line is "owned" by those two players). A line is one of the 3 rows, 3 columns, or 2 diagonals. For single-player lines, each distinct character counts only once, regardless of how many lines it appears in. For two-player lines, each unordered pair of distinct characters (e.g., `{'A','B'}` is the same as `{'B','A'}`) counts only once, regardless of how many lines contain that exact pair. The function must be deterministic and handle any valid input; assume the grid contains only uppercase letters and there are no empty strings.

#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

int main() {
    // Test 1: All same character - one single win (A), no double wins
    std::vector<std::string> grid1 = {"AAA", "AAA", "AAA"};
    auto result1 = countWinningLines(grid1);
    assert(result1.first == 1);
    assert(result1.second == 0);

    // Test 2: No winning lines at all (all distinct in every line)
    std::vector<std::string> grid2 = {"ABC", "DEF", "GHI"};
    auto result2 = countWinningLines(grid2);
    assert(result2.first == 0);
    assert(result2.second == 0);

    // Test 3: Mix of single and double wins, with duplicates across lines
    std::vector<std::string> grid3 = {"AAO", "OBO", "AAO"}; 
    // Lines: row0 "AAO" -> double {A,O}; row1 "OBO" -> double {B,O}; row2 "AAO" -> duplicate of {A,O}
    // col0 "AOA" -> double {A,O}; col1 "ABB" -> double {A,B}; col2 "OOO" -> single O
    // diag1 "ABO" -> no win; diag2 "OBA" -> double {A,B} (duplicate)
    // Singles: only 'O' (from col2) -> 1
    // Doubles: {A,O}, {B,O}, {A,B} -> 3
    auto result3 = countWinningLines(grid3);
    assert(result3.first == 1);
    assert(result3.second == 3);

    // Test 4: Multiple single wins with same character? Not possible since if 'X' appears in two lines, still counts as one distinct single.
    std::vector<std::string> grid4 = {"XXX", "OOO", "XXX"};
    // Rows: "XXX" (X), "OOO" (O), "XXX" (X duplicate) -> singles: X and O -> 2
    // Columns: col0 "XOX" -> double {X,O}; col1 "XOX" -> duplicate; col2 "XOX" -> duplicate -> doubles: {X,O} -> 1
    // Diagonals: "XOX" -> double {X,O} duplicate; "XXX" -> single X duplicate
    auto result4 = countWinningLines(grid4);
    assert(result4.first == 2);
    assert(result4.second == 1);

    // Test 5: Only double wins, all pairs distinct
    std::vector<std::string> grid5 = {"AAB", "BCC", "ABA"};
    // rows: "AAB" -> {A,B}; "BCC" -> {B,C}; "ABA" -> {A,B} duplicate
    // cols: "ABA" -> {A,B} dup; "ACA" -> {A,C}; "BCA" -> {A,B,C} -> no win
    // diag1 "ACA" -> {A,C} dup; diag2 "BBA" -> {A,B} dup
    // unique pairs: {A,B}, {B,C}, {A,C} -> 3
    auto result5 = countWinningLines(grid5);
    assert(result5.first == 0);
    assert(result5.second == 3);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <string>
#include <algorithm>

// Count distinct single-player and two-player winning lines in a 3x3 Tic-Tac-Toe grid.
// Returns {singles, doubles} where singles = number of distinct characters that appear in a full line,
// and doubles = number of distinct unordered pairs that together fill a full line.
std::pair<int, int> countWinningLines(const std::vector<std::string>& grid) {
    int singles = 0;
    int doubles = 0;
    std::vector<char> seen_single;
    std::vector<std::vector<char>> seen_double;

    // Helper lambda to process a single line (given as a 3-character string).
    auto processLine = [&](const std::string& line) {
        // Single-player check: all three characters identical
        if (line[0] == line[1] && line[1] == line[2]) {
            if (std::find(seen_single.begin(), seen_single.end(), line[0]) == seen_single.end()) {
                seen_single.push_back(line[0]);
                ++singles;
            }
        }

        // Two-player check: exactly two distinct characters
        std::vector<char> unique_chars;
        for (char c : line) {
            if (std::find(unique_chars.begin(), unique_chars.end(), c) == unique_chars.end()) {
                unique_chars.push_back(c);
            }
        }
        if (unique_chars.size() == 2) {
            std::sort(unique_chars.begin(), unique_chars.end());
            if (std::find(seen_double.begin(), seen_double.end(), unique_chars) == seen_double.end()) {
                seen_double.push_back(unique_chars);
                ++doubles;
            }
        }
    };

    // Check rows
    for (int row = 0; row < 3; ++row) {
        std::string line = grid[row];
        processLine(line);
    }

    // Check columns
    for (int col = 0; col < 3; ++col) {
        std::string line;
        line += grid[0][col];
        line += grid[1][col];
        line += grid[2][col];
        processLine(line);
    }

    // Check main diagonal (top-left to bottom-right)
    std::string diag1;
    diag1 += grid[0][0];
    diag1 += grid[1][1];
    diag1 += grid[2][2];
    processLine(diag1);

    // Check anti-diagonal (top-right to bottom-left)
    std::string diag2;
    diag2 += grid[0][2];
    diag2 += grid[1][1];
    diag2 += grid[2][0];
    processLine(diag2);

    return {singles, doubles};
}

// The solution iterates over all 8 possible lines in the grid: 3 rows (indexed by row), 3 columns (indexed by column), and the two diagonals. For each line, we extract the three characters into a temporary string or vector. Then we check two conditions:
// 1. **Single-player win**: If all three characters are equal (use `std::count` or simply compare characters), and the character has not been seen before in a single-win line, increment the single count and add the character to a `seen_single` set (or vector for simplicity).
// 2. **Two-player win**: Collect the unique characters in the line. If there are exactly 2 unique characters, this line qualifies as a two-player win. Sort the two characters to enforce a canonical order (so `{'A','B'}` and `{'B','A'}` are treated the same), then check if this pair has been seen before. If not, increment the double count and store the pair in a `seen_double` vector of vectors (or use a set of strings for simpler comparison).
//
// Edge cases: A line like `"AAA"` is a single win but not a double win (since only one unique character). A line like `"ABA"` is not a single win but is a double win because exactly two unique characters appear (`A` and `B`). A line like `"ABC"` is neither. The input grid is always 3x3, so no need for bounds checking. The function should not modify the input (use `const` references). Time complexity: O(8 * constant work) = O(1) because the grid size is fixed; the only variable is the number of distinct lines (8), and each line processes at most 3 characters. Space complexity: O(1) because we store at most 3 single characters and up to 3 double pairs (since there are only 8 lines total, but the actual maximum distinct pairs is limited by the alphabet; still O(1) as the grid is fixed size).

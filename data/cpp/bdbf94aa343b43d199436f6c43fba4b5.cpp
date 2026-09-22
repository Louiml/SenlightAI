/*
Write a C++ function `int countAdjacentPairs(const std::vector<std::string>& grid)` that takes a rectangular grid of characters (each string is a row, all rows same length) and returns the number of horizontally adjacent pairs of identical non-space characters. Here, "horizontally adjacent" means two cells in the same row and in consecutive columns (left neighbor and right neighbor). Only count pairs where both cells contain a non-space character (i.e., not `' '`). For example, in the row `"aa bb"`, there is one pair (`"aa"`), and in `"aaa"` there are two pairs (`(0,1)` and `(1,2)`). The grid may be empty or contain empty rows, but if non‑empty, all rows have equal length. The function must not modify the input and must use `const` correctly.
*/
#include <vector>
#include <string>

// Count horizontally adjacent pairs of identical non-space characters in a grid.
// Each row is a string, all rows have the same length (if any row exists).
int countAdjacentPairs(const std::vector<std::string>& grid) {
    int totalPairs = 0;
    if (grid.empty()) {
        return 0;
    }
    const size_t numRows = grid.size();
    const size_t numCols = grid[0].size();
    for (size_t r = 0; r < numRows; ++r) {
        // Because all rows have same length, grid[r].size() == numCols.
        for (size_t c = 0; c + 1 < grid[r].size(); ++c) {
            char left = grid[r][c];
            char right = grid[r][c + 1];
            if (left != ' ' && right != ' ' && left == right) {
                ++totalPairs;
            }
        }
    }
    return totalPairs;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or via header).

int main() {
    // Empty grid
    std::vector<std::string> grid0;
    assert(countAdjacentPairs(grid0) == 0);

    // Single cell
    std::vector<std::string> grid1 = {"a"};
    assert(countAdjacentPairs(grid1) == 0);

    // Single row with spaces and repeated letters
    std::vector<std::string> grid2 = {"aa bb"};
    assert(countAdjacentPairs(grid2) == 1);

    // Single row all same letters
    std::vector<std::string> grid3 = {"aaa"};
    assert(countAdjacentPairs(grid3) == 2);

    // Multiple rows with different patterns
    std::vector<std::string> grid4 = {"abba", "a a ", "  cc"};
    // row0: "abba": pairs: a?b? no, b?b? yes (index1-2) -> 1
    // row1: "a a ": pairs: a?space? no, space?a? no, a?space? no -> 0
    // row2: "  cc": space?space? no, space?c? no, c?c? yes -> 1
    assert(countAdjacentPairs(grid4) == 2);

    // Single row with spaces only
    std::vector<std::string> grid5 = {"   "};
    assert(countAdjacentPairs(grid5) == 0);

    // Two identical non-space but different case
    std::vector<std::string> grid6 = {"aA"};
    assert(countAdjacentPairs(grid6) == 0);

    // Large same-letter row
    std::vector<std::string> grid7 = {"xxxx"};
    assert(countAdjacentPairs(grid7) == 3);

    // Mixed with spaces in middle of identical letters
    std::vector<std::string> grid8 = {"a a"};
    assert(countAdjacentPairs(grid8) == 0);

    // Row of length 2 identical non‑space
    std::vector<std::string> grid9 = {"bb"};
    assert(countAdjacentPairs(grid9) == 1);

    return 0;
}
// The problem is straightforward: iterate over each row, and for each column index from 0 to `cols-2`, check if `grid[r][c]` equals `grid[r][c+1]` and both are not spaces. If so, increment the count. Edge cases: empty grid (return 0), empty rows (return 0), rows of length 1 (no pairs), and spaces that break pairs (e.g., `"a a"` gives 0 pairs because the middle is space). Since we only look forward to the next column, each pair is counted exactly once. Time complexity is O(rows*cols) because we touch every cell once. Space complexity is O(1) auxilliary (ignoring the input itself). No special memory allocation is needed.

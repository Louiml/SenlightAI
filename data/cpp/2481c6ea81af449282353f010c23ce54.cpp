/*
Write a C++ function named `findEscapeColumn` that accepts a 2D character grid representing a room layout, where each cell is either `'O'` (an obstacle) or `'X'` (empty space). The grid has `n` rows and `m` columns, with values `1 ≤ n, m ≤ 100`. The function must determine the leftmost column (1-indexed) that contains no obstacles in any of its rows, meaning every cell in that column must be `'X'`. If such a column exists, return its 1-based column number. If no such column exists, return `-1`. The function should take the grid as a `std::vector<std::string>` (where each string is a row of characters) and return an `int`.
*/

#include <vector>
#include <string>

// Returns the leftmost 1-indexed column containing no 'O' obstacles,
// or -1 if every column contains at least one obstacle.
int findEscapeColumn(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return -1;
    }
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    for (int col = 0; col < cols; ++col) {
        bool clear = true;
        for (int row = 0; row < rows; ++row) {
            if (grid[row][col] == 'O') {
                clear = false;
                break;
            }
        }
        if (clear) {
            return col + 1; // 1-indexed
        }
    }
    return -1;
}

#include <cassert>
#include <vector>
#include <string>

// Declare the function (already defined above, but included here for completeness)
int findEscapeColumn(const std::vector<std::string>& grid);

int main() {
    std::vector<std::string> grid1 = {"XXX", "XXX", "XXX"};
    assert(findEscapeColumn(grid1) == 1);

    std::vector<std::string> grid2 = {"OXX", "XOX", "XXO"};
    assert(findEscapeColumn(grid2) == 3);

    std::vector<std::string> grid3 = {"OOO", "OOO"};
    assert(findEscapeColumn(grid3) == -1);

    std::vector<std::string> grid4 = {"X", "X", "O"};
    assert(findEscapeColumn(grid4) == 1);

    std::vector<std::string> grid5 = {"O", "O"};
    assert(findEscapeColumn(grid5) == -1);

    std::vector<std::string> grid6 = {"XO", "OX"};
    assert(findEscapeColumn(grid6) == -1);

    std::vector<std::string> grid7 = {"XOX", "XXX", "XOX"};
    assert(findEscapeColumn(grid7) == 1);

    std::vector<std::string> grid8 = {"OXO", "XOX", "OXO"};
    assert(findEscapeColumn(grid8) == 2);

    // Test with single row, many columns
    std::vector<std::string> grid9 = {"OXXO"};
    assert(findEscapeColumn(grid9) == 2);

    // Test with single column, all X
    std::vector<std::string> grid10 = {"X", "X", "X"};
    assert(findEscapeColumn(grid10) == 1);

    return 0;
}

// The solution iterates column by column from index 0 to m-1. For each column, it checks every row to see if any cell is `'O'`. As soon as an obstacle is found in that column, the column is skipped and we move to the next one. The first column where all rows contain `'X'` is the answer, and we return its 1-based index (i.e., column index + 1). If we complete the loop without finding such a column, we return `-1`. Edge cases include: a column with all `'X'` but empty grid (though n,m ≥ 1 so not applicable), columns with mixed characters (only `'O'` or `'X'` are given), and the case where the leftmost eligible column is at position m (last column). The algorithm runs in O(n*m) time because in the worst case we examine every cell, and uses O(1) auxiliary space beyond the input.

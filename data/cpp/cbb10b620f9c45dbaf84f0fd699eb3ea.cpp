/*
Write a C++ function named `countValidPathsWithObstacles` that takes a 2D vector of integers, where `0` represents an empty cell and `1` represents an obstacle, and returns the number of unique paths from the top-left corner to the bottom-right corner. You may only move **right** or **down** at each step. If the start or end cell is blocked by an obstacle, the function must return `0`. The grid can be empty (0 rows or 0 columns), and in such cases the function must return `0`. The function must be `const`-correct, take the grid by `const std::vector<std::vector<int>>&`, and not modify the input.
*/
#include <vector>

// Count unique paths from top-left to bottom-right in a grid with obstacles.
// 0 = empty cell, 1 = obstacle. Only right/down moves allowed.
int countValidPathsWithObstacles(const std::vector<std::vector<int>>& obstacleGrid) {
    int rows = static_cast<int>(obstacleGrid.size());
    if (rows == 0) return 0;
    int cols = static_cast<int>(obstacleGrid[0].size());
    if (cols == 0) return 0;

    // DP table, initialized to 0.
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (obstacleGrid[row][col] == 1) {
                dp[row][col] = 0; // blocked cell
            } else if (row == 0 && col == 0) {
                dp[row][col] = 1; // start cell is free
            } else if (row == 0) {
                dp[row][col] = dp[row][col - 1]; // only from left
            } else if (col == 0) {
                dp[row][col] = dp[row - 1][col]; // only from above
            } else {
                dp[row][col] = dp[row][col - 1] + dp[row - 1][col];
            }
        }
    }

    return dp[rows - 1][cols - 1];
}
#include <cassert>
#include <vector>

int main() {
    // Basic 2x2 grid without obstacles
    std::vector<std::vector<int>> grid1 = {{0, 0}, {0, 0}};
    assert(countValidPathsWithObstacles(grid1) == 2);

    // 2x2 grid with an obstacle in the middle
    std::vector<std::vector<int>> grid2 = {{0, 1}, {0, 0}};
    assert(countValidPathsWithObstacles(grid2) == 1);

    // Start cell blocked
    std::vector<std::vector<int>> grid3 = {{1, 0}, {0, 0}};
    assert(countValidPathsWithObstacles(grid3) == 0);

    // End cell blocked
    std::vector<std::vector<int>> grid4 = {{0, 0}, {0, 1}};
    assert(countValidPathsWithObstacles(grid4) == 0);

    // 3x3 grid with an obstacle in the middle
    std::vector<std::vector<int>> grid5 = {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}};
    assert(countValidPathsWithObstacles(grid5) == 2);

    // Single row with obstacles
    std::vector<std::vector<int>> grid6 = {{0, 1, 0}};
    assert(countValidPathsWithObstacles(grid6) == 0);

    // Single column with no obstacles
    std::vector<std::vector<int>> grid7 = {{0}, {0}, {0}};
    assert(countValidPathsWithObstacles(grid7) == 1);

    // Empty grid (no rows)
    std::vector<std::vector<int>> grid8;
    assert(countValidPathsWithObstacles(grid8) == 0);

    // Grid with zero columns
    std::vector<std::vector<int>> grid9 = {{}, {}};
    assert(countValidPathsWithObstacles(grid9) == 0);

    // Larger grid: 4x4 with obstacles
    std::vector<std::vector<int>> grid10 = {
        {0, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 0}
    };
    assert(countValidPathsWithObstacles(grid10) == 4);
}
// The solution uses dynamic programming with a 2D table `dp` of the same dimensions as the grid, where `dp[row][col]` stores the number of valid paths to reach that cell. We iterate row by row, column by column. For the starting cell `(0,0)`, the number of paths is `1` if it’s not an obstacle, otherwise `0`. For the first row (row == 0) and first column (col == 0), paths can only come from one direction, so we carry forward the previous cell’s count only if the current cell is not an obstacle; if an obstacle is encountered, the count becomes `0` and remains `0` for all subsequent cells in that row/column because you cannot jump over an obstacle. For all other cells, if the current cell is not an obstacle, `dp[row][col] = dp[row][col-1] + dp[row-1][col]`, else it’s `0`. The answer is `dp[rows-1][cols-1]`. Edge cases include empty grids (return 0), blocked start or end (return 0), and grids with only one row or one column (handled by the boundary logic). Time complexity is `O(rows * cols)` and space complexity is `O(rows * cols)` for the DP table; this can be optimized to `O(cols)` by using a 1D array, but the table approach is clearer and acceptable for this task.

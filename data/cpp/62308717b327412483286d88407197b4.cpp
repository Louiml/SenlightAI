/*
Implement a C++ function `int minPathSum(const std::vector<std::vector<int>>& grid)` that takes a non-empty rectangular grid of non-negative integers (each row has the same length, and the grid has at least one cell) and returns the minimum sum of all numbers along a path from the top-left corner `(0,0)` to the bottom-right corner `(M-1,N-1)`. You may only move either down or right at any step. The function must compute the result using dynamic programming with a 2D DP table, and must not modify the input grid. The function should be `const`-correct and handle any valid grid size, including a single row or a single column.
*/

#include <vector>
#include <algorithm>

int minPathSum(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // DP table of same dimensions as grid
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

    // Initialize top-left cell
    dp[0][0] = grid[0][0];

    // Fill first row (only reachable from left)
    for (int j = 1; j < cols; ++j) {
        dp[0][j] = grid[0][j] + dp[0][j-1];
    }

    // Fill first column (only reachable from above)
    for (int i = 1; i < rows; ++i) {
        dp[i][0] = grid[i][0] + dp[i-1][0];
    }

    // Fill the rest: choose the smaller of top and left neighbors
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            dp[i][j] = grid[i][j] + std::min(dp[i-1][j], dp[i][j-1]);
        }
    }

    return dp[rows-1][cols-1];
}

#include <cassert>
#include <vector>

// Include the solution function declaration here (or define above)

int main() {
    // Single cell
    assert(minPathSum({{5}}) == 5);

    // Single row
    assert(minPathSum({{1, 2, 3}}) == 6);

    // Single column
    assert(minPathSum({{1}, {2}, {3}}) == 6);

    // Standard 2x2 case
    assert(minPathSum({{1, 2}, {1, 1}}) == 3);

    // Larger 3x3 case
    assert(minPathSum({{1, 3, 1}, {1, 5, 1}, {4, 2, 1}}) == 7);

    // Rectangular 2x3 case
    assert(minPathSum({{1, 2, 3}, {4, 5, 6}}) == 12);

    // All zeros
    assert(minPathSum({{0, 0}, {0, 0}}) == 0);

    // Large values
    assert(minPathSum({{100, 1}, {1, 1}}) == 3);

    // Mixed larger grid
    std::vector<std::vector<int>> grid = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    assert(minPathSum(grid) == 33);

    return 0;
}

// The core idea is to build a DP table where `dp[i][j]` represents the minimum path sum to reach cell `(i,j)` from the top-left. Since movement is restricted to only right and down, the path to `(i,j)` must come from either the cell above `(i-1,j)` or the cell to the left `(i,j-1)`, and the optimal choice is the smaller of these two plus the current cell's value. The DP table is initialized by setting `dp[0][0] = grid[0][0]`, then filling the first row by cumulative sums from left to right (since only from the left), and the first column by cumulative sums from top to bottom (since only from above). After that, iterate through the remaining cells in row-major order, computing `dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])`. The answer is `dp[M-1][N-1]`. Edge cases: if the grid has only one cell, the function returns that value directly after initialization; if there is only one row or one column, the loops handle it naturally without accessing out-of-bounds. Time complexity is O(M*N) because each cell is computed once, and space complexity is O(M*N) due to the DP table.

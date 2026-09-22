/*
Write a C++ free function `int minPathSum(const std::vector<std::vector<int>>& grid)` that, given a non-empty grid of non-negative integers (at least 1×1), returns the minimum sum of a path from the top-left cell to the bottom-right cell. You may only move right or down at each step. The grid dimensions are not necessarily square, and the function must handle rectangular grids efficiently and without modifying the input. Assume the input matrix is always non-empty, so you don’t need to validate for size zero, but you should handle cases with a single row or a single column gracefully. Your solution must use dynamic programming with optimal substructure, and it should not use recursion to avoid stack overflow for large grids.
*/
#include <vector>
#include <algorithm>

// Returns the minimum path sum from top-left to bottom-right in the grid.
// Only moves right or down are allowed. The grid is assumed non-empty.
int minPathSum(const std::vector<std::vector<int>>& grid) {
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    // DP table: dp[i][j] = minimum sum to reach cell (i,j).
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

    dp[0][0] = grid[0][0];

    // Initialize first row: only possible to come from the left.
    for (int j = 1; j < cols; ++j) {
        dp[0][j] = dp[0][j - 1] + grid[0][j];
    }

    // Initialize first column: only possible to come from above.
    for (int i = 1; i < rows; ++i) {
        dp[i][0] = dp[i - 1][0] + grid[i][0];
    }

    // Fill the rest using the optimal substructure.
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            dp[i][j] = grid[i][j] + std::min(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    return dp[rows - 1][cols - 1];
}
#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above or included here.)
int minPathSum(const std::vector<std::vector<int>>& grid);

int main() {
    // 1x1 grid
    assert(minPathSum({{5}}) == 5);

    // 1x3 row
    assert(minPathSum({{1, 2, 3}}) == 6);

    // 3x1 column
    assert(minPathSum({{1}, {2}, {3}}) == 6);

    // 2x2 simple
    assert(minPathSum({{1, 2}, {1, 1}}) == 3);

    // Standard 3x3 example
    assert(minPathSum({{1, 3, 1}, {1, 5, 1}, {4, 2, 1}}) == 7);

    // All same values
    assert(minPathSum({{2, 2}, {2, 2}}) == 6);

    // Larger rectangular 2x3
    assert(minPathSum({{1, 1, 1}, {1, 1, 1}}) == 3);

    // Check no modification of input (hard to assert, but at least correctness)
    std::vector<std::vector<int>> grid = {{1, 2}, {3, 4}};
    assert(minPathSum(grid) == 7);
    assert(grid[0][0] == 1 && grid[1][1] == 4);

    return 0;
}
// The problem is a classic dynamic programming minimum-cost path in a 2D grid. The key observation is that the minimum sum to reach each cell depends only on the minimum sum to reach its top neighbor and left neighbor. We define a DP table `dp` of the same dimensions, where `dp[i][j]` stores the minimum sum to reach cell `(i,j)` from the top-left. We initialize `dp[0][0] = grid[0][0]`. For the first row (`i=0, j>0`), the only way to reach is from the left, so we accumulate the row sums. For the first column (`j=0, i>0`), the only way is from above, so we accumulate column sums. For all other cells, `dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])`, because we choose the cheaper of the two possible predecessor paths. The answer is `dp[m-1][n-1]`. Edge cases include single-row or single-column grids, where the loop structure naturally only processes the first row or first column initialization, and the nested loop may be skipped entirely. The time complexity is O(m·n) because we fill each cell once, and the space complexity is O(m·n) for the DP table. The algorithm is iterative and safe for large grids. No modification to the input is made, and the function is `const`-correct because it takes the grid by const reference.

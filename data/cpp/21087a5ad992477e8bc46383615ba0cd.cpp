/*
Write a C++ function `int minPathSum(const std::vector<std::vector<int>>& grid)` that, given a non-empty `m x n` grid filled with non-negative integers, returns the minimum sum of a path from the top-left corner `(0,0)` to the bottom-right corner `(m-1, n-1)`, where movement is allowed only to the right or downward at each step. The grid dimensions are at least 1x1. The function must handle grids with up to, say, 200x200 cells and values up to 1000, and must not modify the input grid. The solution must use dynamic programming (bottom-up or top-down with memoization) and be correct for all edge cases including single-row, single-column, and 1x1 grids.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Computes minimum path sum from top-left to bottom-right with right/down moves.
int minPathSum(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));
    
    for (int r = rows - 1; r >= 0; --r) {
        for (int c = cols - 1; c >= 0; --c) {
            if (r == rows - 1 && c == cols - 1) {
                dp[r][c] = grid[r][c];
            } else {
                int right = (c + 1 < cols) ? dp[r][c + 1] : INT_MAX;
                int down  = (r + 1 < rows) ? dp[r + 1][c] : INT_MAX;
                dp[r][c] = grid[r][c] + std::min(right, down);
            }
        }
    }
    return dp[0][0];
}

#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // 1x1 grid
    assert(minPathSum({{5}}) == 5);
    // Single row
    assert(minPathSum({{1, 2, 3}}) == 6);
    // Single column
    assert(minPathSum({{1}, {2}, {3}}) == 6);
    // Standard 2x2
    assert(minPathSum({{1, 2}, {1, 1}}) == 3);
    // Standard 3x3
    assert(minPathSum({{1, 3, 1}, {1, 5, 1}, {4, 2, 1}}) == 7);
    // Larger grid with zeros
    assert(minPathSum({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}) == 0);
    // Mixed values
    assert(minPathSum({{1, 2, 3}, {4, 5, 6}}) == 12);
    // Grid with one large value
    assert(minPathSum({{1, 100}, {1, 1}}) == 3);
    // All equal values
    assert(minPathSum({{2, 2}, {2, 2}}) == 6);
    // Large dimensions (not exhaustive but basic)
    std::vector<std::vector<int>> big(100, std::vector<int>(100, 1));
    assert(minPathSum(big) == 199); // path length 199 cells each of value 1
    return 0;
}

// The optimal path problem satisfies the optimal substructure property: the minimum path sum to reach cell `(r,c)` depends only on the minimum path sums of the cells from which you can reach it (`(r-1,c)` from above and `(r,c-1)` from left). Using bottom-up dynamic programming, we compute the minimum sum from the destination backwards: start from `(m-1, n-1)` and fill the DP table from bottom-right to top-left. For each cell, the value is `grid[r][c]` plus the minimum of the value to its right (if within bounds) and the value below (if within bounds); for the destination itself, it’s just `grid[m-1][n-1]`. Edge cases: when `m==1` or `n==1`, only one direction exists, so the DP still works because out-of-bounds accesses are guarded. The time complexity is `O(m*n)` and space complexity is `O(m*n)` for the DP table (could be optimized to `O(n)` but not required). The algorithm avoids recursion overhead and integer overflow since values are non-negative and bounded.

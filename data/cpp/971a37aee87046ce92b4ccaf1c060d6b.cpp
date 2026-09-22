Write a C++ function `int longestIncreasingPath(const vector<vector<int>>& matrix)` that returns the length of the longest strictly increasing path in a rectangular grid of integers. A path consists of cells where you can move only up, down, left, or right (no diagonals), and each next cell’s value must be strictly greater than the current cell’s value. The matrix may be empty (return 0), may have only one row or one column, and may contain duplicate values; however, the path must be strictly increasing, so equal or smaller values cannot be visited. The function should be efficiently memoized to avoid recomputing subproblems. Do not modify the input matrix. Provide a free function (not a class method) with the above signature.

#include <cassert>
#include <vector>

// The solution function is declared in this file (not repeated here).
int main() {
    // Empty matrix
    assert(longestIncreasingPath({}) == 0);
    
    // Single cell
    assert(longestIncreasingPath({{5}}) == 1);
    
    // Simple increasing row
    assert(longestIncreasingPath({{1, 2, 3}}) == 3);
    
    // Simple increasing column
    assert(longestIncreasingPath({{1}, {2}, {3}}) == 3);
    
    // Classic example from LeetCode: longest path is 4 (6->9, 9->? actually 1->2->6->9)
    std::vector<std::vector<int>> grid1 = {{3, 4, 5},
                                           {3, 2, 6},
                                           {2, 2, 1}};
    assert(longestIncreasingPath(grid1) == 4);
    
    // All equal values -> no strictly increasing move
    std::vector<std::vector<int>> grid2 = {{1, 1}, {1, 1}};
    assert(longestIncreasingPath(grid2) == 1);
    
    // Decreasing grid -> no moves, answer 1
    assert(longestIncreasingPath({{9, 8, 7}, {6, 5, 4}}) == 1);
    
    // Zigzag path
    std::vector<std::vector<int>> grid3 = {{1, 2}, {4, 3}};
    assert(longestIncreasingPath(grid3) == 4); // 1->2->3->4
    
    // Single row with negative and positive numbers
    assert(longestIncreasingPath({{-5, -1, 0, 3}}) == 4);
    
    // Duplicate values in path not allowed
    std::vector<std::vector<int>> grid4 = {{1, 2, 2},
                                           {2, 3, 4}};
    assert(longestIncreasingPath(grid4) == 4); // path 1->2->3->4 (choose one 2)
    
    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>

// Compute the length of the longest strictly increasing path in a grid.
int longestIncreasingPath(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    
    std::vector<std::vector<int>> memo(rows, std::vector<int>(cols, 0));
    
    // Depth-first search from cell (r, c) with previous value 'prev'.
    // Returns the length of the longest increasing path starting at (r, c).
    auto dfs = [&](int r, int c, int prev, auto&& dfs_ref) -> int {
        if (r < 0 || r >= rows || c < 0 || c >= cols) return 0;
        if (matrix[r][c] <= prev) return 0;  // not strictly increasing
        if (memo[r][c] != 0) return memo[r][c];
        
        int best = 1;
        best = std::max(best, 1 + dfs_ref(r, c + 1, matrix[r][c], dfs_ref));
        best = std::max(best, 1 + dfs_ref(r, c - 1, matrix[r][c], dfs_ref));
        best = std::max(best, 1 + dfs_ref(r + 1, c, matrix[r][c], dfs_ref));
        best = std::max(best, 1 + dfs_ref(r - 1, c, matrix[r][c], dfs_ref));
        
        memo[r][c] = best;
        return best;
    };
    
    int overallMax = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            overallMax = std::max(overallMax, dfs(i, j, INT_MIN, dfs));
        }
    }
    return overallMax;
}

// We use depth-first search (DFS) with memoization. For each cell `(i, j)`, define `dp[i][j]` as the length of the longest strictly increasing path starting at that cell. The recurrence is: `dp[i][j] = 1 + max(dp[neighbor])` over all four neighbors whose value is strictly greater than `matrix[i][j]`. If no such neighbor exists, `dp[i][j] = 1`. A global memo table (initialized to 0) stores results; if we encounter a cell already computed, we return its stored value. We start DFS from every cell, using an initial sentinel value of `INT_MIN` for the previous cell so that the first cell is always accepted. The answer is the maximum over all cells. Edge cases: empty matrix → return 0; matrix with one cell → return 1; all equal values → return 1 because no strictly increasing move exists. Memoization ensures each cell is visited at most once per recursion tree, but in total each cell’s DFS runs once, so time complexity is O(R*C) because each cell’s result is computed once and reused. Space complexity is O(R*C) for the memo table plus the implicit call stack depth of at most O(R*C) in the worst case (a snake-like strictly increasing path). Overall O(R*C) time and O(R*C) space.

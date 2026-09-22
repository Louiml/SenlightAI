Write a C++ function `long long uniquePaths(int n, int m)` that computes the number of distinct paths from the top-left corner to the bottom-right corner of an `n` by `m` grid, where movement is allowed only down or right. The function must handle grids up to 25x25, and the result may exceed 32-bit integers, so use `long long`. The grid coordinates are 1-indexed (e.g., `n=1, m=1` represents a single cell, where the path count is 1). The function should be efficient for multiple queries, but for this task, it will be called once per test case. Ensure the result is correct for all `n, m` in the range 1 to 25.
// The problem is a classic combinatorics/counting problem: the number of monotonic paths from one corner to another in a grid. The state `dp[i][j]` represents the number of paths to reach cell `(i,j)` from `(1,1)`. Since movement is only right and down, `dp[i][j] = dp[i-1][j] + dp[i][j-1]`, because to reach `(i,j)` you must come from either the cell above or the cell to the left. Base cases: `dp[i][1] = 1` and `dp[1][j] = 1` for all `i,j`, because there is exactly one path along the top row or left column (only moving right or only moving down). The recurrence holds for `i>1` and `j>1`. Precompute the table for all `i,j` up to 25 once (or compute on demand). Edge cases: when `n=1` or `m=1`, the result is always 1, which the base cases handle. For larger grids, the count grows quickly (e.g., 25x25 yields over 10^14), so `long long` (64-bit) is sufficient. Time complexity: O(25*25) = O(625) for precomputation, and O(1) per query. Space: O(625) for the table. The solution avoids recursion and is iterative.
#include <vector>
#include <cstdint>

// Precompute number of unique paths for grids up to 25x25.
// The table is static so it's computed only once per program run.
long long uniquePaths(int n, int m) {
    static std::vector<std::vector<long long>> dp(26, std::vector<long long>(26, 0));
    static bool initialized = false;
    
    if (!initialized) {
        // Base cases: only one way to reach any cell in first row or first column.
        for (int i = 1; i <= 25; ++i) {
            dp[i][1] = 1;
            dp[1][i] = 1;
        }
        // Fill the rest using recurrence.
        for (int i = 2; i <= 25; ++i) {
            for (int j = 2; j <= 25; ++j) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
        initialized = true;
    }
    
    return dp[n][m];
}
#include <cassert>

int main() {
    // Single-cell grid.
    assert(uniquePaths(1, 1) == 1);
    // Single row or column.
    assert(uniquePaths(1, 10) == 1);
    assert(uniquePaths(10, 1) == 1);
    // 2x2 grid.
    assert(uniquePaths(2, 2) == 2);
    // 3x3 grid.
    assert(uniquePaths(3, 3) == 6);
    // 2x3 grid.
    assert(uniquePaths(2, 3) == 3);
    // 3x2 grid.
    assert(uniquePaths(3, 2) == 3);
    // Symmetry check.
    assert(uniquePaths(5, 7) == uniquePaths(7, 5));
    // Known value for 5x5 (central binomial coefficient C(8,4)=70).
    assert(uniquePaths(5, 5) == 70);
    // Larger grid, verify results fit in long long and are consistent.
    assert(uniquePaths(25, 25) > 0);
    // Known value for 10x10: C(18,9) = 48620.
    assert(uniquePaths(10, 10) == 48620);
    
    return 0;
}

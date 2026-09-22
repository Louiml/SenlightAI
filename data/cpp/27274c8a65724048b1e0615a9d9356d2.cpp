Write a C++ function named `countUniquePaths` that takes two positive integers `m` and `n`, representing the dimensions of an `m` by `n` grid, and returns the number of unique paths a robot can take from the top-left cell to the bottom-right cell, moving only right or down at each step. The function must handle grids down to 1×1 (where exactly 1 path exists) and must correctly compute paths for sizes up to at least 20×20 (where the result fits in a 32-bit integer). Your implementation should use dynamic programming with a 2D table to avoid redundant recursive calls.
#include <cassert>

int main() {
    // 1x1 grid: only one cell.
    assert(countUniquePaths(1, 1) == 1);

    // 1xN or Mx1 grids: always 1.
    assert(countUniquePaths(1, 5) == 1);
    assert(countUniquePaths(7, 1) == 1);

    // 2x2 grid: two paths (right-down, down-right).
    assert(countUniquePaths(2, 2) == 2);

    // 3x3 grid: known result 6.
    assert(countUniquePaths(3, 3) == 6);

    // 3x7 grid: known result C(8,2)=28.
    assert(countUniquePaths(3, 7) == 28);

    // 10x10 grid: result fits in int.
    assert(countUniquePaths(10, 10) == 48620);

    // Symmetry: paths(m,n) == paths(n,m).
    assert(countUniquePaths(4, 5) == countUniquePaths(5, 4));

    // Larger: 20x20 gives 35345263800, but that overflows int.
    // Instead test 15x15: fits in int (40116600).
    assert(countUniquePaths(15, 15) == 40116600);

    // 0 or negative inputs are not valid per problem, but we can
    // test that function still returns a consistent result for m=0.
    // However, the task specifies positive integers, so these are skipped.
    // Final sanity check.
    assert(countUniquePaths(2, 3) == 3);
}
#include <vector>
#include <cstdint>

// Returns the number of unique paths from top-left to bottom-right
// of an m x n grid, moving only right or down.
int countUniquePaths(int m, int n) {
    // Use a 2D vector initialized to 0.
    std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));

    // First column: only one way (move down).
    for (int i = 0; i < m; ++i) {
        dp[i][0] = 1;
    }
    // First row: only one way (move right).
    for (int j = 0; j < n; ++j) {
        dp[0][j] = 1;
    }

    // Fill the rest using the recurrence.
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    // The answer is in the bottom-right cell.
    return dp[m - 1][n - 1];
}
// The core observation is that the number of ways to reach any cell `(i,j)` is the sum of ways to reach the cell directly above `(i-1,j)` and the cell directly to the left `(i,j-1)`, because the robot can only move down or right. For cells in the first row or first column, there is exactly one path: the robot can only move along that row or column without deviating. Thus, we initialize the first row and first column to 1. Then we iterate from row 1 to m-1 and column 1 to n-1, filling each cell with the sum of its top and left neighbors. The answer is the value at `(m-1, n-1)`. Edge cases: if either `m` or `n` is 1, the grid is a single straight line, and the answer is always 1—our initialization handles this automatically because the DP loops won't execute. Time complexity is O(m*n) for filling the table, and space complexity is O(m*n) for the DP table. This is optimal asymptotically since we must consider every cell at least once.

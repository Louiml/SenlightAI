// Write a C++ function `int uniquePaths(int m, int n)` that calculates the number of unique paths a robot can take from the top-left corner (0,0) to the bottom-right corner (m-1, n-1) of an `m x n` grid, moving only right or down at each step. Use memoization (top-down dynamic programming) to avoid recomputing overlapping subproblems. The function must handle cases where `m` or `n` is 1 (only one row or one column, meaning exactly 1 path), and large grids (ensure no integer overflow by using `int` but note that the result fits within `int` for typical test cases like m,n ≤ 15). Assume `m` and `n` are positive integers (≥1) and at most 15 for safety, though the algorithm works for larger values as long as the count fits in `int`.
The problem is a classic combinatorial counting problem. A top-down recursive approach with memoization is used. The recursive function `findways(m, n, i, j, dp)` explores from position `(i,j)`:
- Base case: if `(i,j)` is the bottom-right corner `(m-1, n-1)`, return 1 (there is exactly one way to be at the destination).
- If `i` or `j` goes out of bounds (i.e., `i >= m` or `j >= n`), return 0 because no valid path exists from there.
- If the result for `(i,j)` is already computed (dp[i][j] != -1), return it.
- Otherwise, compute the sum of moving right (`findways(m,n,i,j+1)`) and moving down (`findways(m,n,i+1,j)`), store it in `dp[i][j]`, and return it.

Edge cases: When `m == 1` or `n == 1`, the robot can only move in one direction, so there is exactly 1 path; the recursion handles this because from any cell except the last, only one valid move exists (the other goes out of bounds). For `m` and `n` both positive, the DP table of size `(m+1) x (n+1)` is used to include boundary indices; dp[m][*] and dp[*][n] are never valid positions but allocated for simplicity. Time complexity is O(m*n) because each cell is computed once, and each call does O(1) work. Space complexity is O(m*n) for the memoization table plus O(m*n) recursion stack in the worst case (though recursion depth is at most m+n-1, so space is O(m+n) for stack, but the table dominates, so O(m*n) overall).
#include <vector>

// Count unique paths from top-left to bottom-right in an m x n grid,
// moving only right or down, using memoized recursion.
int findWays(int m, int n, int i, int j, std::vector<std::vector<int>>& dp) {
    // Base case: reached bottom-right corner
    if (i == m - 1 && j == n - 1) {
        return 1;
    }
    // Out of bounds: no valid path from here
    if (i >= m || j >= n) {
        return 0;
    }
    // Memoization: return if already computed
    if (dp[i][j] != -1) {
        return dp[i][j];
    }
    // Move right and down, sum the results
    int rightWays = findWays(m, n, i, j + 1, dp);
    int downWays  = findWays(m, n, i + 1, j, dp);
    dp[i][j] = rightWays + downWays;
    return dp[i][j];
}

// Public function: returns number of unique paths.
int uniquePaths(int m, int n) {
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, -1));
    return findWays(m, n, 0, 0, dp);
}
#include <cassert>

int main() {
    // Basic grids
    assert(uniquePaths(1, 1) == 1);   // Single cell
    assert(uniquePaths(1, 5) == 1);   // One row
    assert(uniquePaths(5, 1) == 1);   // One column

    // Standard cases
    assert(uniquePaths(2, 2) == 2);   // Right-Down or Down-Right
    assert(uniquePaths(3, 2) == 3);   // 3 possible paths
    assert(uniquePaths(3, 7) == 28);  // Known result
    assert(uniquePaths(4, 4) == 20);  // Known result

    // Larger grid (fits in int)
    assert(uniquePaths(10, 10) == 48620); // C(18,9) = 48620

    // Asymmetric grid
    assert(uniquePaths(5, 6) == 126); // C(9,4) = 126
}

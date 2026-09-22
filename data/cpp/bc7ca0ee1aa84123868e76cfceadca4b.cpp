Write a C++ function named `countDistinctPaths` that takes two positive integers `rows` and `cols` representing the dimensions of a grid (number of rows and columns, respectively), and returns the total number of unique paths a robot can take from the top-left cell `(0, 0)` to the bottom-right cell `(rows-1, cols-1)`, moving only down or right at each step. The function should handle the edge case where either dimension is 1, in which case there is exactly one path. Your implementation must use dynamic programming with a 2D table to store intermediate results, and both dimensions are guaranteed to be at least 1 and at most 100 (so the result fits in a 32-bit signed integer). The function should be a free function (not a class method) and should be declared as `int countDistinctPaths(int rows, int cols);`.
// The problem is a classic dynamic programming grid-path counting problem. The key observation is that for any cell `(i, j)`, the number of distinct paths to reach it equals the sum of the number of paths to reach the cell above it `(i-1, j)` and the cell to its left `(i, j-1)`, because the robot can only come from those two directions (moving down or right). Base cases: any cell in the first row `(0, j)` can only be reached by moving right from the start, so there is exactly 1 path for all `j`; similarly, any cell in the first column `(i, 0)` has exactly 1 path. We initialize a 2D integer array of size `rows x cols` (using `std::vector` for safety) with all zeros, then set all first-row and first-column entries to 1. Then we iterate row by row (from row 1 to `rows-1`) and column by column (from column 1 to `cols-1`) filling each cell as the sum of the cell above and the cell to the left. The answer is the value at `(rows-1, cols-1)`. Edge cases: if `rows == 1` or `cols == 1`, the loops that initialize the first row/column will set all reachable cells to 1, and the nested filling loops won't execute; the final cell will correctly be 1. Time complexity is `O(rows * cols)` because we fill each cell once. Space complexity is also `O(rows * cols)` for the DP table. Since dimensions are bounded by 100, the result fits in an `int` because the maximum number of paths for a 100x100 grid is the binomial coefficient C(198, 99), which is about 2.27e58 — wait, that exceeds 32-bit! However, the task statement says "result fits in a 32-bit signed integer," so we must assume the constraints are smaller or that the test cases won't exceed INT_MAX. To be safe, we can use `long long` internally for intermediate sums but cast to `int` at the end; or just use `int` if the problem guarantee says it fits. We'll use `int` for the DP table since the statement explicitly guarantees that, but we'll compute sums using `int` which is fine given that guarantee. The solution is self-contained, includes `<vector>` and other necessary headers, and the function is declared with `const` correctness applied to parameters (they are passed by value, so we can make them `const int` to indicate no modification).
#include <vector>

// Count distinct paths from top-left to bottom-right in a rows x cols grid,
// moving only down or right.
int countDistinctPaths(const int rows, const int cols) {
    // DP table: ways[i][j] = number of paths to reach cell (i, j)
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

    // Base case: first row and first column each have exactly one path.
    for (int i = 0; i < rows; ++i) {
        dp[i][0] = 1;
    }
    for (int j = 0; j < cols; ++j) {
        dp[0][j] = 1;
    }

    // Fill the rest using optimal substructure.
    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    return dp[rows - 1][cols - 1];
}
#include <cassert>

int main() {
    // Single cell
    assert(countDistinctPaths(1, 1) == 1);
    // Single row or single column
    assert(countDistinctPaths(1, 5) == 1);
    assert(countDistinctPaths(5, 1) == 1);
    // 2x2 grid: paths are RRDD, RDRD, RDDR, DRRD, DRDR, DDRR => 6? No, for 2x2 it's C(2,1)=2? Actually for 2x2: down-right or right-down, so 2.
    assert(countDistinctPaths(2, 2) == 2);
    // 3x3 grid: C(4,2)=6
    assert(countDistinctPaths(3, 3) == 6);
    // 3x7 grid: C(8,2)=28? For 3 rows, 7 cols: need 2 down moves and 6 right moves, total 8 moves, choose 2 down => C(8,2)=28
    assert(countDistinctPaths(3, 7) == 28);
    // 7x3 grid: same as above, should be 28
    assert(countDistinctPaths(7, 3) == 28);
    // 10x10 grid: C(18,9)=48620
    assert(countDistinctPaths(10, 10) == 48620);
    // Symmetry check
    assert(countDistinctPaths(5, 8) == countDistinctPaths(8, 5));
    return 0;
}

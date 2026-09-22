Write a C++ function `countSquareSubmatrices` that takes a non-empty 2D vector of integers (`vector<vector<int>>`) containing only `0`s and `1`s, and returns the total number of square submatrices having all ones. The function must operate on a `const` reference to the matrix, use dynamic programming, and handle edge cases such as a 1×1 matrix, a fully zero matrix, and a fully ones matrix. The function should be efficient for matrices up to 300×300. Do not modify the input.

// The solution uses dynamic programming where `dp[i][j]` represents the side length of the largest all-ones square whose bottom-right corner is at cell `(i, j)`. For any cell, if the matrix value is `0`, then `dp[i][j] = 0` because no square can end there. If the cell is `1`, the largest square ending there is determined by the minimum of the three neighboring squares: top-left `(i-1,j-1)`, top `(i-1,j)`, and left `(i,j-1)`, plus 1. This works because a square of side `k` ending at `(i,j)` requires the three adjacent cells to support squares of side at least `k-1`. For cells in the first row or first column, any `1` can only form a 1×1 square, so `dp[i][j] = matrix[i][j]` directly (which is `1` if `1`, `0` if `0`). The total number of all-ones squares is the sum of all `dp[i][j]` values, since each cell contributes exactly the number of squares that end at that cell (sizes 1 through `dp[i][j]`). Edge cases: empty matrix is not allowed as per spec, but a matrix with all zeros yields sum 0; a 1×1 matrix with `1` yields 1; a 2×2 matrix of all ones yields 4 squares (one 2×2 and four 1×1). Time complexity is O(m*n), space complexity is O(m*n) for the dp table, though it could be optimized to O(n) using rolling arrays but not required.

#include <vector>
#include <algorithm>

// Count the number of square submatrices with all ones.
int countSquareSubmatrices(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return 0;
    }
    int m = matrix.size();
    int n = matrix[0].size();
    std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));
    int total = 0;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == 1) {
                if (i == 0 || j == 0) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = 1 + std::min({dp[i-1][j-1], dp[i-1][j], dp[i][j-1]});
                }
                total += dp[i][j];
            }
            // If matrix[i][j] == 0, dp[i][j] stays 0, contributes nothing.
        }
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Simple 1x1 with one
    std::vector<std::vector<int>> m1 = {{1}};
    assert(countSquareSubmatrices(m1) == 1);

    // 1x1 with zero
    std::vector<std::vector<int>> m2 = {{0}};
    assert(countSquareSubmatrices(m2) == 0);

    // 2x2 all ones -> four 1x1 + one 2x2 = 5
    std::vector<std::vector<int>> m3 = {{1,1},{1,1}};
    assert(countSquareSubmatrices(m3) == 5);

    // 3x3 all ones -> 9 + 4 + 1 = 14
    std::vector<std::vector<int>> m4 = {{1,1,1},{1,1,1},{1,1,1}};
    assert(countSquareSubmatrices(m4) == 14);

    // Mixed matrix
    std::vector<std::vector<int>> m5 = {
        {0,1,1,1},
        {1,1,1,1},
        {0,1,1,1}
    };
    // Compute manually: dp values (0 for zero cells, else min+1)
    // Row0: [0,1,1,1] -> dp: [0,1,1,1] sum=3
    // Row1: [1,1,1,1] -> dp: [1,1,2,2] sum=6
    // Row2: [0,1,1,1] -> dp: [0,1,2,2] sum=5
    // Total = 3+6+5 = 14
    assert(countSquareSubmatrices(m5) == 14);

    // All zeros matrix
    std::vector<std::vector<int>> m6 = {{0,0},{0,0}};
    assert(countSquareSubmatrices(m6) == 0);

    // Single row with alternating 1s
    std::vector<std::vector<int>> m7 = {{1,0,1,0,1}};
    // Each 1 contributes exactly 1 square
    assert(countSquareSubmatrices(m7) == 3);

    // Single column with pattern
    std::vector<std::vector<int>> m8 = {{1},{1},{0},{1}};
    // Three ones at rows 0,1,3 each contribute 1
    assert(countSquareSubmatrices(m8) == 3);

    // Edge: rectangular non-square
    std::vector<std::vector<int>> m9 = {{1,1},{1,1},{1,1}}; // 3x2
    // dp: row0 [1,1] sum=2; row1 [1,2] sum=3; row2 [1,2] sum=3; total=8
    assert(countSquareSubmatrices(m9) == 8);

    // Large case test with 5x5 all ones: sum of squares 25+16+9+4+1=55
    std::vector<std::vector<int>> m10(5, std::vector<int>(5, 1));
    assert(countSquareSubmatrices(m10) == 55);
}

Write a C++ function that takes an `n x m` binary matrix (where each entry is either 0 or 1) and returns the side length of the largest square submatrix that contains only 1s. The function should accept the matrix as a `const std::vector<std::vector<int>>&` (with all rows having equal length), and return the maximum side length as an `int`. If the matrix is empty or contains no 1s, return 0. Your implementation must be efficient for large matrices and avoid recursion.
#include <cassert>
#include <vector>

// The solution function is declared above (or included from the solution file).
// This test file assumes the function is already defined.

int main() {
    // Empty matrix
    std::vector<std::vector<int>> empty;
    assert(largestSquareOfOnes(empty) == 0);

    // Matrix with zero columns (but with rows)
    std::vector<std::vector<int>> noCols = {{}, {}};
    assert(largestSquareOfOnes(noCols) == 0);

    // All zeros
    std::vector<std::vector<int>> allZeros = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    assert(largestSquareOfOnes(allZeros) == 0);

    // Single 1
    std::vector<std::vector<int>> singleOne = {{1}};
    assert(largestSquareOfOnes(singleOne) == 1);

    // Single 0
    std::vector<std::vector<int>> singleZero = {{0}};
    assert(largestSquareOfOnes(singleZero) == 0);

    // Simple 2x2 all ones
    std::vector<std::vector<int>> twoByTwoOnes = {
        {1, 1},
        {1, 1}
    };
    assert(largestSquareOfOnes(twoByTwoOnes) == 2);

    // 3x4 matrix with a 2x2 square
    std::vector<std::vector<int>> matrix1 = {
        {1, 0, 1, 0},
        {1, 1, 1, 0},
        {1, 1, 1, 0}
    };
    assert(largestSquareOfOnes(matrix1) == 2);

    // 4x4 matrix with a 3x3 square
    std::vector<std::vector<int>> matrix2 = {
        {1, 1, 1, 0},
        {1, 1, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    };
    assert(largestSquareOfOnes(matrix2) == 3);

    // Larger matrix with 4x4 square in corner
    std::vector<std::vector<int>> matrix3 = {
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };
    assert(largestSquareOfOnes(matrix3) == 4);

    // Non-square matrix, no square larger than 1
    std::vector<std::vector<int>> matrix4 = {
        {1, 0},
        {0, 1},
        {1, 0}
    };
    assert(largestSquareOfOnes(matrix4) == 1);

    // Matrix with a single row of ones
    std::vector<std::vector<int>> singleRow = {{1, 1, 1, 1}};
    assert(largestSquareOfOnes(singleRow) == 1); // only 1x1 squares possible

    // Matrix with a single column of ones
    std::vector<std::vector<int>> singleCol = {{1}, {1}, {1}};
    assert(largestSquareOfOnes(singleCol) == 1);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the side length of the largest square submatrix consisting entirely of 1s.
// Input: mat - a rectangular binary matrix (all rows have equal length).
// Output: maximum side length of an all-ones square, or 0 if none exists.
int largestSquareOfOnes(const std::vector<std::vector<int>>& mat) {
    if (mat.empty() || mat[0].empty()) return 0;
    int n = static_cast<int>(mat.size());
    int m = static_cast<int>(mat[0].size());

    // curr[j] = dp value for current row i at column j
    // next[j] = dp value for row i+1 at column j
    std::vector<int> curr(m + 1, 0); // extra column for j = m (sentinel, always 0)
    std::vector<int> next(m + 1, 0);

    int maxSide = 0;

    // Iterate from bottom row to top row
    for (int i = n - 1; i >= 0; --i) {
        // Process columns from right to left
        for (int j = m - 1; j >= 0; --j) {
            if (mat[i][j] == 1) {
                int right = curr[j + 1];       // dp[i][j+1]
                int diagonal = next[j + 1];    // dp[i+1][j+1]
                int down = next[j];            // dp[i+1][j]
                curr[j] = 1 + std::min({right, diagonal, down});
                maxSide = std::max(maxSide, curr[j]);
            } else {
                curr[j] = 0;
            }
        }
        // Move current row to next for the next iteration (i-1)
        std::swap(curr, next);
        // Reset curr to all zeros for the next row above? Actually we need to clear curr,
        // but after swap, curr holds old next values. We'll clear it in the outer loop
        // by using a fresh assignment or fill with 0.
        std::fill(curr.begin(), curr.end(), 0);
    }

    return maxSide;
}
// The classic dynamic programming solution computes the side length of the largest all-ones square ending at each cell `(i,j)`. Define `dp[i][j]` as the side length of the largest square of 1s whose bottom-right corner is at cell `(i,j)`. The recurrence is: if `mat[i][j] == 1`, then `dp[i][j] = 1 + min(dp[i][j+1], dp[i+1][j+1], dp[i+1][j])` when computing from bottom-right to top-left (or equivalently `1 + min(dp[i-1][j], dp[i-1][j-1], dp[i][j-1])` when computing top-left to bottom-right). If `mat[i][j] == 0`, then `dp[i][j] = 0`. The answer is the maximum value in `dp`. We can optimize space to O(m) by using two rows (current and next) when iterating from bottom to top, since each cell depends on the cell to its right (same row, next column) and the two cells below (next row, same column and next column). Edge cases: empty matrix, zero rows or columns, all zeros, single cell, and matrices where the largest square touches the boundaries. This solution runs in O(n·m) time and O(m) auxiliary space (or O(n·m) if using a full DP table).

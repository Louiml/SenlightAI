Given a 2D grid of characters where `'1'` represents a filled cell and `'0'` represents an empty cell, write a C++ function that returns the area of the largest square containing only `'1'`s. The input grid is guaranteed to have at least one row and one column (non-empty). The function should handle a single-cell grid correctly and work efficiently for large grids. The result must be an integer representing the total number of cells in that maximal square (i.e., side length squared).
// The solution uses dynamic programming. We define `dp[i][j]` as the side length of the largest square whose bottom-right corner is at cell `(i, j)`. For a cell containing `'1'`, the value of `dp[i][j]` is `1` plus the minimum of the three neighboring `dp` values: above, left, and diagonally above-left. This works because a square of side `k` ending at `(i,j)` exists exactly when the squares of side `k-1` ending at those three neighbors exist. If the cell contains `'0'`, `dp[i][j]` is `0`. We track the maximum `dp` value encountered, and the final answer is that maximum squared. Edge cases: when `i=0` or `j=0`, only the cell itself can form a square, so `dp[i][j]` is `1` if the cell is `'1'`. For a 1×1 grid with `'1'`, the answer is `1`; with `'0'`, it is `0`. The algorithm processes each cell exactly once, so time complexity is O(m×n), and space complexity is O(m×n) for the DP table. A space optimization using two rows is possible, but the straightforward approach is sufficient and clearer.
#include <vector>
#include <algorithm>

// Returns the area of the largest square of '1's in the grid.
int maximalSquareArea(const std::vector<std::vector<char>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));
    
    int maxSide = 0;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == '1') {
                if (i == 0 || j == 0) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                }
                maxSide = std::max(maxSide, dp[i][j]);
            }
        }
    }
    
    return maxSide * maxSide;
}
#include <cassert>
#include <vector>

// The solution function is defined above (included here for completeness).
int maximalSquareArea(const std::vector<std::vector<char>>& matrix);

int main() {
    // Single cell with '1'
    assert(maximalSquareArea({{'1'}}) == 1);
    // Single cell with '0'
    assert(maximalSquareArea({{'0'}}) == 0);
    // 2x2 all ones
    assert(maximalSquareArea({{'1','1'},{'1','1'}}) == 4);
    // 3x3 with a 2x2 square
    assert(maximalSquareArea({{'1','0','1'},{'1','1','0'},{'1','1','0'}}) == 4);
    // 4x4 with a 3x3 square
    std::vector<std::vector<char>> grid = {
        {'1','0','1','0'},
        {'1','1','1','1'},
        {'1','1','1','1'},
        {'1','1','1','1'}
    };
    assert(maximalSquareArea(grid) == 9);
    // No ones
    assert(maximalSquareArea({{'0','0'},{'0','0'}}) == 0);
    // Large square, 5x5 with only last cell missing (still 5 side? Actually 5x5 all but one still not square? Test with perfect 5x5)
    std::vector<std::vector<char>> full5(5, std::vector<char>(5, '1'));
    assert(maximalSquareArea(full5) == 25);
    // Rectangular grid, 2x5 where the 2x2 block works
    assert(maximalSquareArea({{'1','1','0','1','1'},{'1','1','0','1','1'}}) == 4);
    
    return 0;
}

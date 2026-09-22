// Write a C++ function `countSquareSubmatrices` that takes a 2D vector of integers (each entry is either `0` or `1`) and returns the total number of square submatrices consisting entirely of `1`s. The input matrix is guaranteed to be non-empty (at least 1 row and 1 column). A square submatrix is any contiguous square block of cells (side length ≥ 1) that is entirely filled with `1`s. For example, in a matrix `[[1,0,1],[1,1,0],[1,1,0]]`, the total count is 7 (there are five 1×1 squares, one 2×2 square at the bottom-left, and one 2×2 square at the top-left). The function must be `const`‑correct and should not modify the input matrix.

// The solution uses dynamic programming. Define `dp[i][j]` as the side length of the largest square of all `1`s whose bottom‑right corner is at cell `(i-1, j-1)` (shifting by 1 to avoid separate edge‑case checks). For each cell `(i,j)` in the original matrix, if `matrix[i][j] == 1`, then `dp[i+1][j+1] = 1 + min(dp[i][j+1], dp[i+1][j], dp[i][j])`. This recurrence works because the largest square ending at that cell is limited by the smallest square among the top, left, and top‑left neighboring cells. The total answer is the sum of all `dp[i+1][j+1]` values, since each square of side length `k` contributes `1` to the count for every `k` from 1 up to its side length. Important edge cases: matrix of all zeros returns 0; single‑row or single‑column matrices work correctly because the `dp` dimensions are `(rows+1) × (cols+1)` with zero‑initialized borders. Time complexity is `O(rows × cols)` and space complexity is `O(rows × cols)` for the `dp` table.

#include <vector>
#include <algorithm>

// Count the total number of square submatrices consisting entirely of 1s.
int countSquareSubmatrices(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    
    // dp[i+1][j+1] = side length of the largest all‑1 square ending at (i, j)
    std::vector<std::vector<int>> dp(rows + 1, std::vector<int>(cols + 1, 0));
    int total = 0;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 1) {
                dp[i + 1][j + 1] = 1 + std::min({dp[i][j + 1], dp[i + 1][j], dp[i][j]});
                total += dp[i + 1][j + 1];
            }
        }
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Test case 1: all zeros
    std::vector<std::vector<int>> m1 = {{0,0,0},{0,0,0}};
    assert(countSquareSubmatrices(m1) == 0);

    // Test case 2: single 1
    std::vector<std::vector<int>> m2 = {{1}};
    assert(countSquareSubmatrices(m2) == 1);

    // Test case 3: 2x2 all ones → 4 + 1 = 5
    std::vector<std::vector<int>> m3 = {{1,1},{1,1}};
    assert(countSquareSubmatrices(m3) == 5);

    // Test case 4: example from prompt
    std::vector<std::vector<int>> m4 = {{1,0,1},{1,1,0},{1,1,0}};
    assert(countSquareSubmatrices(m4) == 7);

    // Test case 5: single row
    std::vector<std::vector<int>> m5 = {{1,1,1}};
    assert(countSquareSubmatrices(m5) == 3);

    // Test case 6: single column with pattern
    std::vector<std::vector<int>> m6 = {{1},{1},{0},{1}};
    // squares: (0,0), (1,0) => two 1×1 squares only (no 2×2 possible) → 2
    assert(countSquareSubmatrices(m6) == 2);

    // Test case 7: asymmetric with 3x3 all ones → 9 + 4 + 1 = 14
    std::vector<std::vector<int>> m7 = {{1,1,1},{1,1,1},{1,1,1}};
    assert(countSquareSubmatrices(m7) == 14);
}

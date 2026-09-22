Write a C++ function `countGridPaths(int rows, int cols)` that returns the number of distinct ways a traveler can move from the top-left corner to the bottom-right corner of a `rows × cols` grid using only moves down or right. The function must handle grids of any size up to at least 20 × 20 (values fit in 64-bit integers), and must return 0 for any invalid dimension (e.g., rows ≤ 0 or cols ≤ 0). The result should be computed iteratively using a 2D DP table where each cell stores the number of paths to reach that cell, with the starting cell set to 1. The function must be self-contained, using only standard headers, and must not rely on recursion.
#include <cassert>
#include <iostream>

// Prototype of the solution function (declared here for the test; assumed present elsewhere).
long long countGridPaths(int rows, int cols);

int main() {
    // Small valid grids.
    assert(countGridPaths(1, 1) == 1);      // Already at destination.
    assert(countGridPaths(1, 3) == 1);      // Only one way: move right twice.
    assert(countGridPaths(3, 1) == 1);      // Only one way: move down twice.
    assert(countGridPaths(2, 2) == 2);      // Two paths: RD or DR.
    assert(countGridPaths(3, 3) == 6);      // Known result for 3x3 grid.
    assert(countGridPaths(3, 2) == 3);      // Known result from snippet.
    // Larger grid (5x5) matches binomial coefficient combination(8,4).
    assert(countGridPaths(5, 5) == 70);
    // Invalid dimensions.
    assert(countGridPaths(0, 5) == 0);
    assert(countGridPaths(5, 0) == 0);
    assert(countGridPaths(-2, 3) == 0);
    assert(countGridPaths(0, 0) == 0);
    // Large grid within 64-bit range (20x20 gives combination(38,19) = 35345263800).
    assert(countGridPaths(20, 20) == 35345263800LL);
    // Symmetry: paths(rows, cols) == paths(cols, rows).
    assert(countGridPaths(4, 7) == countGridPaths(7, 4));
    std::cout << "All tests passed!\n";
    return 0;
}
#include <vector>

// Count distinct paths from top-left to bottom-right moving only down or right.
// Returns 0 for invalid dimensions (rows <= 0 or cols <= 0).
long long countGridPaths(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return 0;
    }
    // DP table of size (rows+1) x (cols+1) initialized to 0.
    std::vector<std::vector<long long>> table(rows + 1, std::vector<long long>(cols + 1, 0));
    table[1][1] = 1;  // Starting cell.
    for (int i = 0; i <= rows; ++i) {
        for (int j = 0; j <= cols; ++j) {
            long long current = table[i][j];
            if (i + 1 <= rows) {
                table[i + 1][j] += current;
            }
            if (j + 1 <= cols) {
                table[i][j + 1] += current;
            }
        }
    }
    return table[rows][cols];
}
// The problem is a classic dynamic programming counting problem. The key observation: to reach any cell (i, j), the traveler must come from either the cell above (i-1, j) or the cell to the left (i, j-1). Therefore, the number of ways to reach (i, j) equals the sum of ways to reach those two predecessors. We initialize a (rows+1) × (cols+1) table with all zeros (extra row/column to simplify boundary checks), set `table[1][1] = 1` (1-indexed) as the base case (the starting cell). Then we iterate over all cells from index 0 to rows and 0 to cols; for each cell, if moving down (i+1 ≤ rows) is valid, we add the current cell's value to the cell below; if moving right (j+1 ≤ cols) is valid, we add it to the cell to the right. This propagates counts forward. The final answer is `table[rows][cols]`. Edge cases: if rows or cols is 0 or negative, return 0 (since no valid grid). For 1×1 grid, the answer is 1 (the traveler is already at the destination). Time complexity is O(rows × cols) because we visit each cell once; space complexity is also O(rows × cols) for the DP table. The result fits in a 64-bit integer for grids up to about 20×20 (central binomial coefficients). We use `long long` to avoid overflow for larger grids within typical limits.

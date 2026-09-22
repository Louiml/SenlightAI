// Write a C++ function named `minimumFallingPathSum` that takes a non-empty square matrix of non-negative integers (represented as `std::vector<std::vector<int>>`) and returns the minimum sum of a falling path through the matrix. A falling path starts at any element in the first row and moves down one row at a time, choosing the element directly below, or diagonally down-left, or diagonally down-right (i.e., from column `j` in row `i`, you can move to row `i+1` columns `j-1`, `j`, or `j+1` as long as they are within bounds). The path ends at the last row. The function must not modify the input matrix and should use `const` references for the parameter. The matrix is guaranteed to be square (MxM) and have at least one row.
The problem is a classic dynamic programming (DP) problem where the optimal value for reaching a cell depends only on the three possible cells in the row above. Since moving is always down one row, we can compute the minimum path sum to reach each cell in the current row based on the previous row's computed values. We do not need to store the entire DP table—only two rows (previous and current) are needed because each transition only looks at the previous row. Initialize the previous row as the first row of the input. For each subsequent row, for each column `j`, compute `best = min(prev[max(0,j-1)], prev[j], prev[min(M-1,j+1)])`, then add the current cell's value. After processing all rows, the answer is the minimum value in the last computed row. Edge cases include a 1x1 matrix (answer is that single element), and boundary columns where only two or one valid previous cells exist (handled by clamping indices with `max` and `min`). Time complexity is **O(M²)** where M is the side length of the matrix. Space complexity is **O(M)** for the two row vectors (or **O(M)** if we use two vectors of size M).
#include <vector>
#include <algorithm>
#include <climits>

int minimumFallingPathSum(const std::vector<std::vector<int>>& grid) {
    int m = grid.size();
    if (m == 0) return 0;
    int n = grid[0].size(); // square, but we handle general

    // prev holds minimum sums to reach each cell in the previous row
    std::vector<int> prev = grid[0];
    
    for (int i = 1; i < m; ++i) {
        std::vector<int> curr(n, 0);
        for (int j = 0; j < n; ++j) {
            int best = INT_MAX;
            // check j-1, j, j+1 from prev
            for (int d = -1; d <= 1; ++d) {
                int col = j + d;
                if (col >= 0 && col < n) {
                    best = std::min(best, prev[col]);
                }
            }
            curr[j] = grid[i][j] + best;
        }
        prev = std::move(curr);
    }
    
    return *std::min_element(prev.begin(), prev.end());
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: simple 1x1
    std::vector<std::vector<int>> grid1 = {{5}};
    assert(minimumFallingPathSum(grid1) == 5);

    // Test 2: 2x2
    std::vector<std::vector<int>> grid2 = {{1, 2}, {3, 4}};
    // Paths: 1->3 =4, 1->4=5, 2->3=5, 2->4=6 => min is 4
    assert(minimumFallingPathSum(grid2) == 4);

    // Test 3: 3x3
    std::vector<std::vector<int>> grid3 = {{2, 1, 3}, {6, 5, 4}, {7, 8, 9}};
    // min path: 1 (row0 col1) -> 4 (row1 col2) -> 9 (row2 col2) = 14
    // or 2->5->7=14, 2->5->8=15, etc. correct min is 13? Let's check: 1->4->7=12, 1->4->8=13, 1->4->9=14, 1->5->7=13, 1->5->8=14, 1->5->9=15, 3->4->7=14, etc. Actually min is 12.
    assert(minimumFallingPathSum(grid3) == 12);

    // Test 4: negative? No, constraints say non-negative, but test with zero
    std::vector<std::vector<int>> grid4 = {{0, 0}, {0, 0}};
    assert(minimumFallingPathSum(grid4) == 0);

    // Test 5: larger matrix with known result
    std::vector<std::vector<int>> grid5 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    // min: 1->4->7=12, or 1->5->7=13, etc. min is 12
    assert(minimumFallingPathSum(grid5) == 12);

    // Test 6: 4x4
    std::vector<std::vector<int>> grid6 = {{1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 10}, {10, 11, 12, 13}};
    // For each row, pick smallest of previous three, likely 1+4+7+10=22? but check path: row0 col0=1, row1 col0=4, row2 col0=7, row3 col0=10 sum=22
    assert(minimumFallingPathSum(grid6) == 22);

    return 0;
}

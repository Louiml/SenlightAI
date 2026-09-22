// Write a C++ function `int minPathSum(const std::vector<std::vector<int>>& grid)` that computes the minimum sum of numbers along a path from the top-left corner to the bottom-right corner of a non-empty rectangular grid, moving only right or down at each step. The grid contains non-negative integers, and the path must start at `grid[0][0]` and end at `grid[m-1][n-1]`. The function must handle grids of any dimensions where both `m` and `n` are at least 1. Use dynamic programming with space optimization to achieve `O(n)` auxiliary space, where `n` is the number of columns. The solution must be self-contained, include all necessary headers, and correctly handle edge cases such as a single-row or single-column grid.
// The problem is a classic dynamic programming task: from any cell `(i, j)`, the only valid moves are to `(i+1, j)` and `(i, j+1)`. Therefore, the optimal cost to reach a cell equals the cell's value plus the minimum of the optimal costs of its two potential predecessors: the cell above `(i-1, j)` and the cell to the left `(i, j-1)`. The base case is the start cell `(0,0)` where the cost is the cell's value itself. A straightforward tabulation would use a 2D table of the same dimensions as the grid, filling it row by row and column by column, yielding `O(m*n)` time and `O(m*n)` space. To optimize space, note that to compute the current row, we only need the previous row's values (for "up" moves) and the current row's values already computed for smaller columns (for "left" moves). Thus, we maintain a single vector `prev` of length `n` representing the previous row, and for each row we create a `cur` vector, filling it left to right. After completing a row, we set `prev = cur`. The final answer is `prev[n-1]` after processing all rows. Edge cases: when `m=1` or `n=1`, the path is forced straight along the row/column, and the algorithm naturally handles this because the "up" or "left" moves may not be available (guard with row/col checks). Time complexity is `O(m*n)` and auxiliary space is `O(n)`. The grid is passed by const reference to avoid copying, and the function does not modify the input.
#include <vector>
#include <algorithm>

// Compute the minimum path sum from top-left to bottom-right,
// moving only right or down. Uses O(n) extra space.
int minPathSum(const std::vector<std::vector<int>>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    
    // prev stores the minimum path sums for the previous row.
    std::vector<int> prev(cols, 0);
    
    for (int row = 0; row < rows; ++row) {
        // cur will store the minimum path sums for the current row.
        std::vector<int> cur(cols, 0);
        for (int col = 0; col < cols; ++col) {
            if (row == 0 && col == 0) {
                cur[col] = grid[0][0];
                continue;
            }
            
            const int up = (row >= 1) ? grid[row][col] + prev[col] : 1000000000;
            const int left = (col >= 1) ? grid[row][col] + cur[col - 1] : 1000000000;
            cur[col] = std::min(up, left);
        }
        prev = cur;
    }
    
    return prev[cols - 1];
}
#include <cassert>
#include <vector>

// Include the solution function here.

int main() {
    // Basic 2x2 grid
    std::vector<std::vector<int>> grid1 = {{1, 2}, {3, 4}};
    assert(minPathSum(grid1) == 7); // 1 -> 2 -> 4 or 1 -> 3 -> 4

    // 1x1 grid
    std::vector<std::vector<int>> grid2 = {{5}};
    assert(minPathSum(grid2) == 5);

    // Single row, forced right moves
    std::vector<std::vector<int>> grid3 = {{1, 2, 3}};
    assert(minPathSum(grid3) == 6);

    // Single column, forced down moves
    std::vector<std::vector<int>> grid4 = {{1}, {2}, {3}};
    assert(minPathSum(grid4) == 6);

    // Larger grid with zeros and larger numbers
    std::vector<std::vector<int>> grid5 = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };
    assert(minPathSum(grid5) == 7); // 1 -> 3 -> 1 -> 1 -> 1

    // All zeros
    std::vector<std::vector<int>> grid6 = {{0, 0}, {0, 0}};
    assert(minPathSum(grid6) == 0);

    // 3x3 with a clear optimal path
    std::vector<std::vector<int>> grid7 = {
        {0, 10, 10},
        {1, 0, 10},
        {2, 1, 0}
    };
    assert(minPathSum(grid7) == 1); // 0 -> 1 -> 0 -> 1 -> 0

    // Non-square rectangular grid
    std::vector<std::vector<int>> grid8 = {
        {1, 2, 5},
        {3, 2, 1}
    };
    assert(minPathSum(grid8) == 5); // 1 -> 3 -> 2 -> 1 or 1 -> 2 -> 2 -> 1

    return 0;
}

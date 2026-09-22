Write a C++ function named `ballDropExitColumn` that accepts a 2D grid of integers where each cell contains either `1` (meaning a diagonal board directing a ball to the right) or `-1` (directing a ball to the left). A ball is dropped into the top of each column. When a ball hits a board, it moves diagonally to the adjacent cell in the next row. However, the ball gets stuck if it hits a board at the boundary (leftmost or rightmost column) or if it hits a pair of boards that point in opposite directions (e.g., a `1` next to a `-1` in the same row). The function must return a vector of integers where the `i`-th element is the column index where the ball dropped in column `i` exits the bottom of the grid, or `-1` if it gets stuck. The grid is guaranteed to have at least one row and one column, and each row has the same length. Implement the solution using dynamic programming with a bottom‑up (reversed row) approach, filling a DP table from the last row to the first row, where `dp[row][col]` stores the exit column for a ball starting at that position. The time complexity must be `O(rows * cols)` and space complexity `O(rows * cols)`.

#include <cassert>
#include <vector>

// Assume the solution function is defined above (included in this test file).

int main() {
    // Case 1: Simple grid with clear paths
    std::vector<std::vector<int>> grid1 = {{1,1,1,-1,-1}, {1,1,1,-1,-1}, {-1,-1,-1,1,1}, {1,1,1,1,-1}, {-1,-1,-1,-1,-1}};
    std::vector<int> expect1 = {1, -1, -1, -1, -1};
    assert(ballDropExitColumn(grid1) == expect1);

    // Case 2: Single row, all right boards but ball goes out of bounds
    std::vector<std::vector<int>> grid2 = {{1,1,1,1}};
    std::vector<int> expect2 = {-1, -1, -1, -1};
    assert(ballDropExitColumn(grid2) == expect2);

    // Case 3: Single row, alternating boards that cause collisions
    std::vector<std::vector<int>> grid3 = {{-1,1,-1,1}};
    std::vector<int> expect3 = {-1, -1, -1, -1};
    assert(ballDropExitColumn(grid3) == expect3);

    // Case 4: All left boards, balls exit on left side
    std::vector<std::vector<int>> grid4 = {{-1,-1,-1}, {-1,-1,-1}};
    std::vector<int> expect4 = {-1, 0, 1};
    assert(ballDropExitColumn(grid4) == expect4);

    // Case 5: Single column, any direction leads out of bounds
    std::vector<std::vector<int>> grid5 = {{1}, {-1}, {1}};
    std::vector<int> expect5 = {-1};
    assert(ballDropExitColumn(grid5) == expect5);

    // Case 6: Grid where ball goes straight through to same column
    std::vector<std::vector<int>> grid6 = {{1, -1}, {1, -1}};
    std::vector<int> expect6 = {1, 0};
    assert(ballDropExitColumn(grid6) == expect6);

    // Case 7: Larger grid with mixed results
    std::vector<std::vector<int>> grid7 = {{1,1,1,-1,-1}, {1,1,1,-1,-1}, {-1,-1,-1,1,1}};
    std::vector<int> expect7 = {-1, -1, -1, -1, -1};
    assert(ballDropExitColumn(grid7) == expect7);

    // Case 8: Grid with only one row and one column
    std::vector<std::vector<int>> grid8 = {{1}};
    std::vector<int> expect8 = {-1};
    assert(ballDropExitColumn(grid8) == expect8);

    // Case 9: All balls exit correctly in a zigzag pattern
    std::vector<std::vector<int>> grid9 = {{1, -1}, {-1, 1}, {1, -1}};
    std::vector<int> expect9 = {1, 0};
    assert(ballDropExitColumn(grid9) == expect9);

    // Case 10: Edge where ball hits boundary at the very first move
    std::vector<std::vector<int>> grid10 = {{1, -1, 1}};
    std::vector<int> expect10 = {-1, -1, -1};
    assert(ballDropExitColumn(grid10) == expect10);

    return 0;
}

#include <vector>

// Given a grid of 1's (right) and -1's (left), return the exit column for each
// starting column in the top row, or -1 if the ball gets stuck.
std::vector<int> ballDropExitColumn(const std::vector<std::vector<int>>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    
    // dp[row][col] = exit column for a ball starting at (row, col)
    // dp[rows][col] = col (base case: ball has already exited)
    std::vector<std::vector<int>> dp(rows + 1, std::vector<int>(cols, -1));
    
    // Base case: bottom row (after last physical row)
    for (int col = 0; col < cols; ++col) {
        dp[rows][col] = col;
    }
    
    // Fill DP table from bottom up
    for (int row = rows - 1; row >= 0; --row) {
        for (int col = 0; col < cols; ++col) {
            int direction = grid[row][col];
            int nextCol = col + direction;
            
            // Stuck if out of bounds or boards point opposite directions
            if (nextCol < 0 || nextCol >= cols || 
                grid[row][col] != grid[row][nextCol]) {
                dp[row][col] = -1;
            } else {
                dp[row][col] = dp[row + 1][nextCol];
            }
        }
    }
    
    // Answer for starting at top row (row 0)
    return dp[0];
}

// The core idea is to simulate the ball’s path from the bottom up, computing for each cell the eventual exit column. Since a ball always moves from a cell in row `r` to a cell in row `r+1` (one row below), we can solve this backwards: if we know the exit column for every column in row `r+1`, we can determine the exit for row `r`. For a cell `(r, c)` with direction `d = grid[r][c]`, the next column is `c + d`. There are two ways the ball gets stuck: (1) the next column is out of bounds, or (2) the board at `(r, c)` and the board at `(r, c+d)` point in opposite directions (i.e., `grid[r][c] != grid[r][c+d]`). If neither happens, then the exit column for `(r, c)` equals the exit column for `(r+1, c+d)`. We initialize the last row (index `rows`) as the base case, where `dp[rows][c] = c` because a ball has already exited. Then we iterate from `row = rows-1` down to `0`, filling the DP table. The answer for each starting column is simply `dp[0][col]`. The algorithm runs in `O(rows * cols)` because each cell is processed once, and uses a DP table of size `(rows+1) * cols`, hence `O(rows * cols)` space. Edge cases include a single row (where the ball either exits immediately or gets stuck), single column (all balls must move horizontally so unless the single cell is `1`? Actually with one column, any move goes out of bounds, so all get stuck), and grids with all `1`s or all `-1`s but with boundary issues.

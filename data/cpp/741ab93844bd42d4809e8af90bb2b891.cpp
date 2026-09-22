// Write a C++ function that takes a 2D vector of integers representing a grid where `0` indicates a free cell and `1` indicates an obstacle, and returns the number of unique paths a robot can take from the top-left cell `(0,0)` to the bottom-right cell `(m-1,n-1)`. The robot can only move down or right, and cannot step onto an obstacle. The grid dimensions are at most 100×100, and the answer is guaranteed to fit in a 32-bit signed integer. If the start or end cell is an obstacle, the function must return 0.

The problem is a classic dynamic programming (DP) counting problem. The number of ways to reach any cell `(i,j)` is the sum of ways to reach the cell above it `(i-1,j)` and the cell to its left `(i,j-1)`, but only if the current cell is not an obstacle. Base case: the start cell `(0,0)` has exactly 1 way if it is not an obstacle (since the robot starts there), otherwise 0.  
We can solve using bottom-up DP with a 2D table, or optimize to a 1D rolling array because each row's computation only depends on the previous row and the current row's left neighbor.  
Edge cases:  
- If the start or end is an obstacle, return 0.  
- If the grid is 1×1 and free, return 1.  
- If the grid has obstacles anywhere, paths that would pass through them are excluded.  
Time complexity: O(m·n) because each cell is processed once. Space complexity: O(n) for the rolling array version (or O(m·n) if using full 2D DP). The provided solution uses the O(n) space version.

#include <vector>

// Count unique paths from top-left to bottom-right avoiding obstacles (1 = obstacle).
// Moves allowed: down and right only.
int uniquePathsWithObstacles(const std::vector<std::vector<int>>& obstacleGrid) {
    if (obstacleGrid.empty() || obstacleGrid[0].empty()) return 0;
    int m = obstacleGrid.size();
    int n = obstacleGrid[0].size();

    // If start or end is an obstacle, no path exists.
    if (obstacleGrid[0][0] == 1 || obstacleGrid[m-1][n-1] == 1) return 0;

    // 1D DP array for current row.
    std::vector<int> dp(n, 0);
    dp[0] = 1; // start cell

    for (int i = 0; i < m; ++i) {
        // Iterate row by row; for i=0, dp[0] is already 1.
        for (int j = 0; j < n; ++j) {
            if (obstacleGrid[i][j] == 1) {
                dp[j] = 0; // obstacle blocks all paths through this cell
            } else if (i == 0 && j == 0) {
                // start cell already set; keep 1
            } else {
                // dp[j] currently holds ways from above (previous row), 
                // and dp[j-1] holds ways from left (current row, updated).
                // For i==0, dp[j] was initialized 0, so only left cell counts.
                // For j==0, dp[j-1] is out of bounds, only above counts.
                int above = (i > 0) ? dp[j] : 0;    // from previous row
                int left = (j > 0) ? dp[j-1] : 0;   // from current row, left cell
                dp[j] = above + left;
            }
        }
    }
    return dp[n-1];
}

#include <cassert>
#include <vector>

int main() {
    // Example 1 from problem statement: 3x3 with center obstacle -> 2 paths
    std::vector<std::vector<int>> grid1 = {{0,0,0},{0,1,0},{0,0,0}};
    assert(uniquePathsWithObstacles(grid1) == 2);

    // Example 2: 2x2 with obstacle at (0,1) -> 1 path
    std::vector<std::vector<int>> grid2 = {{0,1},{0,0}};
    assert(uniquePathsWithObstacles(grid2) == 1);

    // No obstacles: 3x3 should have 6 paths (C(4,2)=6)
    std::vector<std::vector<int>> grid3 = {{0,0,0},{0,0,0},{0,0,0}};
    assert(uniquePathsWithObstacles(grid3) == 6);

    // Start cell is obstacle -> 0 paths
    std::vector<std::vector<int>> grid4 = {{1,0},{0,0}};
    assert(uniquePathsWithObstacles(grid4) == 0);

    // End cell is obstacle -> 0 paths
    std::vector<std::vector<int>> grid5 = {{0,0},{0,1}};
    assert(uniquePathsWithObstacles(grid5) == 0);

    // 1x1 free cell -> 1 path
    std::vector<std::vector<int>> grid6 = {{0}};
    assert(uniquePathsWithObstacles(grid6) == 1);

    // 1x1 obstacle -> 0 paths
    std::vector<std::vector<int>> grid7 = {{1}};
    assert(uniquePathsWithObstacles(grid7) == 0);

    // Single row with obstacles splitting: [0,1,0,0] -> only right moves allowed; 
    // from start to end: leftmost is free, but cell 1 blocked, so no path -> 0
    std::vector<std::vector<int>> grid8 = {{0,1,0,0}};
    assert(uniquePathsWithObstacles(grid8) == 0);

    // Single column with obstacle below start: {{0},{1},{0}} -> start free, but above end blocked -> 0
    std::vector<std::vector<int>> grid9 = {{0},{1},{0}};
    assert(uniquePathsWithObstacles(grid9) == 0);

    // Larger grid with one obstacle not blocking all paths: 4x4, obstacle at (1,1)
    std::vector<std::vector<int>> grid10 = {
        {0,0,0,0},
        {0,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    // Without obstacle, paths = C(6,3)=20. With obstacle, subtract paths through (1,1).
    // Paths through (1,1) = ways to (1,1) * ways from (1,1) to end = C(2,1)=2 * C(4,2)=6 = 12.
    // So 20 - 12 = 8.
    assert(uniquePathsWithObstacles(grid10) == 8);

    return 0;
}

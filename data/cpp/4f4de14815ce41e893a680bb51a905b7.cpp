// A robot is initially placed in the top-left corner `(0,0)` of a rectangular grid, and its goal is to reach the bottom-right corner `(m-1,n-1)`. The grid is represented as a 2D vector of integers where `0` denotes an empty cell and `1` denotes an obstacle that cannot be entered. The robot can only move one step down or one step right at a time. Write a C++ function named `uniquePathsWithObstaclesCount` that accepts a 2D vector of integers representing the grid (with at least 1 row and at least 1 column, possibly containing obstacles) and returns the total number of unique paths from the start to the goal, avoiding obstacles. If no valid path exists, return `0`. The function must not modify the input grid and must use constant extra space beyond the grid itself (i.e., it may not allocate a separate DP table of size `m*n`; it must reuse or derive space in a way that is `O(n)` or less, where `n` is the number of columns). Handle edge cases such as a start or goal cell blocked by an obstacle, or a 1x1 grid.

#include <cassert>
#include <vector>

// Declaration of the function (assume it's included above).
int uniquePathsWithObstaclesCount(const std::vector<std::vector<int>>& obstacleGrid);

int main() {
    // Test 1: 3x3 grid without obstacles -> 6 paths (from standard combinatorics).
    std::vector<std::vector<int>> grid1 = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    assert(uniquePathsWithObstaclesCount(grid1) == 6);

    // Test 2: Standard LeetCode example with a single obstacle.
    std::vector<std::vector<int>> grid2 = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    assert(uniquePathsWithObstaclesCount(grid2) == 2);

    // Test 3: Start cell blocked -> 0.
    std::vector<std::vector<int>> grid3 = {{1,0}};
    assert(uniquePathsWithObstaclesCount(grid3) == 0);

    // Test 4: Goal cell blocked -> 0.
    std::vector<std::vector<int>> grid4 = {{0,1}};
    assert(uniquePathsWithObstaclesCount(grid4) == 0);

    // Test 5: 1x1 grid with no obstacle -> 1.
    std::vector<std::vector<int>> grid5 = {{0}};
    assert(uniquePathsWithObstaclesCount(grid5) == 1);

    // Test 6: 1x1 grid with obstacle -> 0.
    std::vector<std::vector<int>> grid6 = {{1}};
    assert(uniquePathsWithObstaclesCount(grid6) == 0);

    // Test 7: 1x4 grid, all free -> only one path (move right all the way).
    std::vector<std::vector<int>> grid7 = {{0,0,0,0}};
    assert(uniquePathsWithObstaclesCount(grid7) == 1);

    // Test 8: Vertical line with an obstacle in the middle -> 0.
    std::vector<std::vector<int>> grid8 = {{0},{1},{0}};
    assert(uniquePathsWithObstaclesCount(grid8) == 0);

    // Test 9: 2x3 grid with obstacles completely blocking paths -> 0.
    std::vector<std::vector<int>> grid9 = {
        {0,1,0},
        {1,0,0}
    };
    assert(uniquePathsWithObstaclesCount(grid9) == 0);

    // Test 10: 3x3 with obstacle path forcing a unique route -> 1.
    std::vector<std::vector<int>> grid10 = {
        {0,1,0},
        {0,1,0},
        {0,0,0}
    };
    assert(uniquePathsWithObstaclesCount(grid10) == 1);

    return 0;
}

#include <vector>

// Count unique paths from top-left to bottom-right avoiding obstacles (1s).
// The robot moves only right or down. Uses 1D DP with O(n) extra space.
// Returns 0 if start or goal is blocked or no path exists.
int uniquePathsWithObstaclesCount(const std::vector<std::vector<int>>& obstacleGrid) {
    if (obstacleGrid.empty() || obstacleGrid[0].empty()) return 0;
    
    const int m = obstacleGrid.size();
    const int n = obstacleGrid[0].size();
    
    // If start or goal is blocked, no path possible.
    if (obstacleGrid[0][0] == 1 || obstacleGrid[m-1][n-1] == 1) return 0;
    
    // dp[j] = number of ways to reach current row, column j.
    std::vector<int> dp(n, 0);
    dp[0] = 1;  // Start cell has 1 way (though it's processed in the first row).
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (obstacleGrid[i][j] == 1) {
                dp[j] = 0;  // Blocked cell contributes no paths.
            } else {
                // Paths from above (dp[j]) plus paths from left (dp[j-1]).
                if (j > 0) {
                    dp[j] += dp[j-1];
                }
                // For j == 0, dp[0] remains as is (only from above).
            }
        }
    }
    
    return dp[n-1];
}

// The problem is a classic dynamic programming (DP) counting problem. The number of distinct paths to any cell `(i,j)` depends only on the cells directly above `(i-1,j)` and to the left `(i,j-1)`, provided those cells are not blocked. To maintain `O(1)` extra space, we use a 1D DP array of size `n` (number of columns). The DP array `dp[j]` stores the number of paths to the current row's column `j` after processing the grid row by row from top to bottom. We initialize `dp[0] = 1` before processing the grid (a virtual starting point). For each cell `(i,j)`, if `grid[i][j] == 1` (obstacle), set `dp[j] = 0` because no path can go through this cell. Otherwise, update `dp[j] = dp[j] + dp[j-1]` (except for the first column where `j==0`, in which case it is just `dp[j]`). This works because `dp[j]` before the update represents the number of paths coming from the previous row (above), and `dp[j-1]` represents paths coming from the left in the current row. After processing all rows, the answer is `dp[n-1]`. Important edge cases: if the starting cell is an obstacle, the function returns 0 (the algorithm handles this because `dp[0]` becomes 0 when the first row's first cell is obstacle); similarly for the goal. For a 1x1 grid with a `0`, the result is 1. Time complexity is `O(m*n)` because each cell is processed exactly once. Space complexity is `O(n)` for the 1D DP array, but to meet the requirement of constant extra space beyond the grid, the problem statement suggests that `O(n)` is acceptable (since `n` is the width, not the total number of cells). The algorithm can be further optimized to `O(1)` extra space by reusing the first row of the grid itself, but to keep it safe and simple, the 1D DP array is considered constant relative to the input size in the context of "extra space beyond the grid itself" — although strictly it's O(n), the problem intends to forbid O(m*n) tables. The solution below uses a `std::vector<int>` of size `n` for clarity.

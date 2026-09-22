Write a C++ function `long long countGridPaths(long long rows, long long cols, const std::vector<std::pair<long long, long long>>& obstacles)` that returns the number of distinct paths from the top-left corner (1,1) to the bottom-right corner (`rows`, `cols`) of a grid. You may only move down or right by exactly one cell at a time. The grid contains `obstacles.size()` blocked cells whose coordinates are given as 1-based pairs (row, column). If the start or end cell is blocked, or if no path exists, return 0. The grid dimensions and obstacle counts can be up to 30, and coordinates are guaranteed valid (within the grid bounds). The function must be self-contained, handle all edge cases gracefully, and avoid integer overflow (use `long long`).

The solution uses dynamic programming on a 2D table. Let `dp[i][j]` be the number of paths from the top-left cell (1,1) to cell (i,j). Initialize all cells to 0, then set `dp[1][1] = 1` (unless blocked). For each obstacle cell, set its `dp` value to 0 and skip it. Then iterate row by row, column by column, and for each non-obstacle cell, add the values from the cell above (if any) and the cell to the left (if any). This works because any path to `(i,j)` must come from `(i-1,j)` or `(i,j-1)`. The answer is `dp[rows][cols]`. Edge cases: if `rows` or `cols` is 0, or if the start/end is an obstacle, return 0. Time complexity is O(rows * cols), and space complexity is O(rows * cols) (can be optimized to O(cols) using rolling arrays, but not strictly required). Use `long long` to handle counts that may exceed 32-bit for grids up to 30x30 (the number of paths can be huge, e.g., C(60,30) ≈ 1.18e17).

#include <vector>
#include <utility>

// Count the number of distinct paths from (1,1) to (rows, cols) on a grid,
// moving only down or right, avoiding the given 1-based obstacle cells.
long long countGridPaths(long long rows, long long cols, const std::vector<std::pair<long long, long long>>& obstacles) {
    if (rows <= 0 || cols <= 0) return 0;

    // Convert to zero-based for internal use.
    long long n = rows, m = cols;
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(m, 0));

    // Mark obstacles.
    for (const auto& obs : obstacles) {
        long long r = obs.first - 1;
        long long c = obs.second - 1;
        if (r >= 0 && r < n && c >= 0 && c < m) {
            dp[r][c] = -1;  // sentinel for blocked
        }
    }

    // Start cell.
    if (dp[0][0] == -1) return 0;
    dp[0][0] = 1;

    // Fill DP table.
    for (long long i = 0; i < n; ++i) {
        for (long long j = 0; j < m; ++j) {
            if (dp[i][j] == -1) {
                dp[i][j] = 0;
                continue;
            }
            if (i > 0 && dp[i-1][j] != -1) dp[i][j] += dp[i-1][j];
            if (j > 0 && dp[i][j-1] != -1) dp[i][j] += dp[i][j-1];
        }
    }

    return dp[n-1][m-1];
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (placeholder for completeness).
long long countGridPaths(long long, long long, const std::vector<std::pair<long long, long long>>&);

int main() {
    // No obstacles, 2x2 grid: paths: RR, DR? Actually 2 paths.
    assert(countGridPaths(2, 2, {}) == 2);

    // 1x1 grid: only start=end, 1 path.
    assert(countGridPaths(1, 1, {}) == 1);

    // 3x3 grid, no obstacles: C(4,2)=6 paths.
    assert(countGridPaths(3, 3, {}) == 6);

    // Blocked center cell (2,2) in 3x3 grid: only 2 paths (right-right-down-down or down-down-right-right? Actually blocked (2,2) eliminates some, leaving 2 paths).
    assert(countGridPaths(3, 3, {{2,2}}) == 2);

    // Start blocked.
    assert(countGridPaths(2, 2, {{1,1}}) == 0);

    // End blocked.
    assert(countGridPaths(2, 2, {{2,2}}) == 0);

    // 5x5 grid with a wall of obstacles forcing a detour.
    assert(countGridPaths(5, 5, {{2,2}, {2,3}, {3,2}, {4,4}}) == 10);

    // Large grid without obstacles: 3x4 grid -> C(5,2)=10? Actually paths = C(rows+cols-2, rows-1) = C(5,2)=10.
    assert(countGridPaths(3, 4, {}) == 10);

    // Grid with all cells blocked except start.
    assert(countGridPaths(2, 2, {{1,2}, {2,1}, {2,2}}) == 0);

    // Negative or zero dimensions.
    assert(countGridPaths(0, 5, {}) == 0);
    assert(countGridPaths(-1, 5, {}) == 0);
}

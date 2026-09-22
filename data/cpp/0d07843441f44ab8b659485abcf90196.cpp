// Write a C++ function `int maxGoldPath(vector<vector<int>>& grid)` that takes a 2D grid of non-negative integers representing gold amounts in cells (0 means no gold and cannot be visited) and returns the maximum amount of gold that can be collected along any single path. A valid path starts at any non-zero cell, can move up, down, left, or right to adjacent cells, cannot re-enter a cell already visited on the same path, and stops when no unvisited neighboring cell contains gold. The grid dimensions can be up to 15×15. The function must not modify the original grid permanently (it may temporarily mark cells visited but must restore them before returning). Assume the grid is rectangular (all rows same length).

// The problem is a classic DFS (depth-first search) on a grid with backtracking. For each cell that contains gold, start a DFS that explores all possible paths that can be taken from that cell. In the DFS, mark the current cell as visited by setting its value to 0, add its gold to the current path sum, update the best sum if the current sum exceeds the best, then recursively explore the four adjacent cells (up, right, down, left). After exploring all neighbors, restore the cell’s original gold value (backtracking) so that other paths through this cell are possible. The base case is when the current cell is out of bounds or has value 0 (either originally empty or already visited). Since each cell can be visited at most once per path, the number of states is bounded by the number of cells, but the total branching can be exponential in the worst case; however, with a 15×15 grid, the constraint is acceptable. Edge cases include: grid with all zeros (return 0), single-cell grid with gold (return that gold), and grids where the best path may not start at a corner or may require revisiting decisions (handled by backtracking). Time complexity: in the worst case, each cell may be the start of a path and for each start, DFS explores all possible paths without revisiting cells. The worst-case time is O(R*C * 4^(R*C)) in theory, but with a 15×15 limit and typical sparse gold distribution, it’s manageable. Space complexity: O(R*C) for the recursion stack depth in the worst case when the path visits all cells.

#include <vector>
#include <algorithm>

// Returns the maximum gold that can be collected along any single path in the grid.
// The grid may be temporarily modified during DFS but is restored before returning.
int maxGoldPath(std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    if (rows == 0) return 0;
    int cols = grid[0].size();
    int best = 0;

    // Depth-first search from (r, c) with current collected gold sum.
    // Returns nothing but updates 'best' as a side effect.
    auto dfs = [&](auto&& dfs_ref, int r, int c, int current) -> void {
        // Out of bounds or cell is empty/visited.
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] == 0) {
            return;
        }

        int gold = grid[r][c];
        grid[r][c] = 0;          // Mark visited
        current += gold;
        best = std::max(best, current);

        // Explore four directions
        dfs_ref(dfs_ref, r - 1, c, current);
        dfs_ref(dfs_ref, r, c + 1, current);
        dfs_ref(dfs_ref, r + 1, c, current);
        dfs_ref(dfs_ref, r, c - 1, current);

        grid[r][c] = gold;       // Backtrack: restore original gold
    };

    // Try starting from every non-zero cell
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != 0) {
                dfs(dfs, i, j, 0);
            }
        }
    }
    return best;
}

#include <cassert>
#include <vector>

// The solution function is defined above; here we test it.
int main() {
    // Test 1: Simple 3x3 grid from typical examples
    std::vector<std::vector<int>> g1 = {{0,6,0},{5,8,7},{0,9,0}};
    assert(maxGoldPath(g1) == 24);

    // Test 2: All zeros
    std::vector<std::vector<int>> g2 = {{0,0},{0,0}};
    assert(maxGoldPath(g2) == 0);

    // Test 3: Single cell with gold
    std::vector<std::vector<int>> g3 = {{7}};
    assert(maxGoldPath(g3) == 7);

    // Test 4: Straight line path
    std::vector<std::vector<int>> g4 = {{1,2,3},{0,0,0}};
    assert(maxGoldPath(g4) == 6);

    // Test 5: Grid where optimal path is a spiral or branching
    std::vector<std::vector<int>> g5 = {{1,0,7},{2,0,6},{3,4,5}};
    assert(maxGoldPath(g5) == 28); // 1+2+3+4+5+6+7 = 28

    // Test 6: Isolated clusters, choose larger cluster
    std::vector<std::vector<int>> g6 = {{10,0,0},{0,0,5},{0,0,5}};
    assert(maxGoldPath(g6) == 10);

    // Test 7: Duplicate paths but different sums
    std::vector<std::vector<int>> g7 = {{0,100,0},{100,1,100},{0,100,0}};
    assert(maxGoldPath(g7) == 201); // 100 + 1 + 100 from any direction

    // Test 8: Large grid but only one path
    std::vector<std::vector<int>> g8(3, std::vector<int>(3, 1));
    assert(maxGoldPath(g8) == 9); // Can visit all 9 cells exactly once

    // Test 9: No gold in some rows
    std::vector<std::vector<int>> g9 = {{0,0,0},{0,4,0},{0,0,0}};
    assert(maxGoldPath(g9) == 4);

    // Test 10: Path that requires turning back to collect more
    std::vector<std::vector<int>> g10 = {{1,2,3},{4,0,5},{6,7,8}};
    assert(maxGoldPath(g10) == 36); // 1+2+3+5+8+7+6+4 = 36

    return 0;
}

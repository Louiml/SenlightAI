Write a C++ function `int minimumMinutesToRotAll(vector<vector<int>>& grid)` that takes a 2D grid of integers where `0` represents an empty cell, `1` represents a fresh orange, and `2` represents a rotten orange. Each minute, any fresh orange that is adjacent (up, down, left, right) to a rotten orange becomes rotten. The function must return the minimum number of minutes required for all fresh oranges to become rotten, or `-1` if it is impossible (i.e., some fresh oranges are unreachable or isolated from any rotten source). The grid has at least one row and one column, and dimensions are up to 10×10. You may modify the input grid. The function should be efficient for the given constraints and handle cases with no fresh oranges (return `0`) and cases with no rotten oranges but fresh ones (return `-1`).
// The problem is a classic multi-source BFS (breadth-first search) for shortest path in an unweighted grid. The key insight is that all initially rotten oranges act as simultaneous sources; each BFS layer corresponds to one minute of rotting. However, a simple BFS would incorrectly treat cells that are completely isolated (surrounded by walls) as rotting after some time, so we first need to detect unreachable fresh oranges.  
//
// The provided snippet uses a DFS pre-check: for every fresh orange `1`, it runs a DFS that marks visited fresh cells as `-1` (reachable) and unreachable ones as `-2`. The DFS returns `true` if the component does not touch any rotten orange or any cell that can reach a rotten orange, meaning it’s an isolated fresh component → return `-1`. After this pre-check, all remaining fresh cells (marked `-1` or original `1`) are reachable. Then we perform a standard BFS from all rotten sources (original `2` and also `-2` cells, which are actually rotten cells found during DFS? Wait, in the snippet, `-2` is used both for unreachable cells and later for newly rotted cells; but the pre-check ensures no `-2` remains except those from the DFS that are actually unreachable and would have already triggered a `-1` return). Care must be taken: in the given code, `-2` is used to mark cells that are unreachable from any rotten source, but after the pre-check, if we didn’t return `-1`, that means no `-2` cells remain because the condition `DFS(...)` returned `true` for at least one fresh cell would have caused an early return. So after the pre-check, the grid contains only `0`, `1`, `-1` (reachable fresh), and `2`. Then BFS treats both `2` and `-2` as rotten, but since no `-2` remains, it’s fine. The BFS runs level by level, incrementing a counter after each level only if new cells were added. The time complexity is O(N*M) for the DFS checks (each cell visited once in its component) and O(N*M) for the BFS, so overall O(N*M). Space complexity is O(N*M) for the recursion stack (DFS) and the queue (BFS), though the DFS recursion depth could be up to N*M in the worst case.  
//
// Edge cases: empty grid (not allowed per constraints), grid with no fresh oranges → return `0`. Grid with fresh oranges but no rotten oranges → return `-1` because they can never rot. Grid where some fresh oranges are completely surrounded by `0` (walls) → return `-1`. Grid where all fresh oranges are adjacent to a rotten orange → return `1`.
#include <vector>
#include <queue>
#include <utility>

// DFS to check if a fresh orange component is isolated from any rotten orange.
// Marks cells: -1 = reachable (can be reached from a rotten source), -2 = unreachable.
bool isIsolatedFromRotten(std::vector<std::vector<int>>& grid, int i, int j, int n, int m) {
    // Out of bounds or empty cell or already processed -> treat as not an obstacle.
    if (i < 0 || j < 0 || i >= n || j >= m || grid[i][j] == 0) return true;
    // If we encounter a rotten orange (2) or a cell already marked as reachable (-1), this component is not isolated.
    if (grid[i][j] == 2 || grid[i][j] == -1) return false;
    // If already marked as unreachable (-2), return true (it's isolated so far).
    if (grid[i][j] == -2) return true;
    
    // Current cell is a fresh orange (1). Mark it as -2 temporarily.
    grid[i][j] = -2;
    
    // Explore neighbors. If any neighbor reaches a rotten orange, this component is not isolated.
    bool up = isIsolatedFromRotten(grid, i-1, j, n, m);
    bool down = isIsolatedFromRotten(grid, i+1, j, n, m);
    bool left = isIsolatedFromRotten(grid, i, j-1, n, m);
    bool right = isIsolatedFromRotten(grid, i, j+1, n, m);
    
    // If all four directions are "true" (meaning no rotten reachable), then this cell is unreachable.
    // Otherwise, mark it as reachable (-1) and propagate that information.
    bool isolated = up && down && left && right;
    grid[i][j] = isolated ? -2 : -1;
    return isolated;
}

// Main function to compute minimum minutes to rot all fresh oranges.
int minimumMinutesToRotAll(std::vector<std::vector<int>>& grid) {
    int n = (int)grid.size();
    int m = (int)grid[0].size();
    
    // Pre-check: detect fresh components that cannot be reached by any rotten orange.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 1) {
                if (isIsolatedFromRotten(grid, i, j, n, m)) {
                    return -1;
                }
            }
        }
    }
    
    // After pre-check, all fresh cells are either 1 (not visited by DFS) or -1 (reachable).
    // Also, any original rotten cells are 2, and there are no -2 cells remaining.
    
    // Multi-source BFS.
    std::queue<std::pair<int,int>> q;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 2) {
                q.push({i, j});
            }
        }
    }
    
    int minutes = 0;
    int freshCount = 0;
    // Count fresh cells (either original 1 or marked as -1).
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 1 || grid[i][j] == -1) freshCount++;
        }
    }
    
    if (freshCount == 0) return 0;
    
    // BFS level by level.
    while (!q.empty() && freshCount > 0) {
        int levelSize = (int)q.size();
        bool rottedAny = false;
        for (int k = 0; k < levelSize; ++k) {
            auto [r, c] = q.front(); q.pop();
            // Four directions.
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < n && nc >= 0 && nc < m && (grid[nr][nc] == 1 || grid[nr][nc] == -1)) {
                    grid[nr][nc] = 2;
                    q.push({nr, nc});
                    freshCount--;
                    rottedAny = true;
                }
            }
        }
        if (rottedAny) minutes++;
    }
    
    // If some fresh oranges remain, it's impossible (shouldn't happen after pre-check, but safety).
    return freshCount == 0 ? minutes : -1;
}
#include <cassert>
#include <vector>

// Declaration of the function to test (assumed to be defined above).
int minimumMinutesToRotAll(std::vector<std::vector<int>>& grid);

int main() {
    // Example from problem statement: returns 4.
    {
        std::vector<std::vector<int>> grid = {{2,1,1},{1,1,0},{0,1,1}};
        assert(minimumMinutesToRotAll(grid) == 4);
    }
    // All oranges already rotten: returns 0.
    {
        std::vector<std::vector<int>> grid = {{2,2,2},{2,2,2}};
        assert(minimumMinutesToRotAll(grid) == 0);
    }
    // No rotten oranges but fresh exist: impossible -> -1.
    {
        std::vector<std::vector<int>> grid = {{1,1,1},{1,0,1}};
        assert(minimumMinutesToRotAll(grid) == -1);
    }
    // Fresh orange isolated by empty cells: -1.
    {
        std::vector<std::vector<int>> grid = {{2,0,1}};
        assert(minimumMinutesToRotAll(grid) == -1);
    }
    // Simple case: fresh adjacent to rotten -> 1 minute.
    {
        std::vector<std::vector<int>> grid = {{2,1}};
        assert(minimumMinutesToRotAll(grid) == 1);
    }
    // Grid with no oranges (all empty): freshCount=0 -> 0.
    {
        std::vector<std::vector<int>> grid = {{0,0},{0,0}};
        assert(minimumMinutesToRotAll(grid) == 0);
    }
    // Multiple fresh components, one reachable, one isolated -> -1.
    {
        std::vector<std::vector<int>> grid = {{2,1,0,1},{0,0,0,1}};
        assert(minimumMinutesToRotAll(grid) == -1);
    }
    // Larger grid requiring multiple minutes.
    {
        std::vector<std::vector<int>> grid = {
            {2,1,1,1},
            {1,1,0,1},
            {0,1,1,1}
        };
        // Simulate: minute1 rot (0,1),(1,0); minute2 rot (0,2),(1,1),(2,1),(2,2); minute3 rot (0,3),(1,3),(2,3) -> 3 minutes.
        assert(minimumMinutesToRotAll(grid) == 3);
    }
    // All fresh but reachable from multiple rotten.
    {
        std::vector<std::vector<int>> grid = {{2,0,2},{1,1,1}};
        // Both rotten adjacent to fresh row; minute1 rots all three fresh? Check: (0,0) rotten -> (1,0) fresh rots; (0,2) rotten -> (1,2) rots; then (1,0) and (1,2) are adjacent to (1,1) so (1,1) rots in same minute? BFS level: at start q has (0,0),(0,2); they all expand in same level, so (1,0),(1,2) become rotten and also (1,1) is adjacent to (1,0) and (1,2) but they are not rotten yet during the same level expansion; actually BFS processes level cells: from (0,0) -> (1,0) is fresh -> mark rotten. From (0,2) -> (1,2) -> mark rotten. No other fresh at that level. So level1 rots (1,0) and (1,2). Then level2 from those, (1,1) rots. So total 2 minutes. 
        assert(minimumMinutesToRotAll(grid) == 2);
    }
    return 0;
}

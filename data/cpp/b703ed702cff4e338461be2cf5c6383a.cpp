// Write a C++ function `int countIslands(const std::vector<std::vector<int>>& grid)` that takes a rectangular grid (2D vector) where each cell contains either `0` (water) or `1` (land). The grid dimensions are at least 1×1 and at most 50×50. An "island" is a maximal group of orthogonally or diagonally adjacent `1`s (8-directional connectivity). The function must return the total number of distinct islands in the grid. The input grid is read-only; the function must not modify it. Ensure your implementation handles cases where the grid is all water, all land, contains only a single cell, and uses 8-directional adjacency correctly.
// The problem is a classic connected-components count with 8-directional adjacency. The main algorithm is Depth-First Search (DFS) or Breadth-First Search (BFS) over each unvisited land cell. Since the grid is passed by const reference, we cannot mutate it, so we maintain a separate `visited` matrix (bool) of the same size, initialized to `false`. For each cell `(i,j)` that is land and not visited, we increment the island count and perform a flood-fill (DFS recursion or an explicit stack) that marks all connected land cells as visited by exploring all 8 neighbors (dx/dy arrays). Edge cases: (1) Empty grid or grid with all zeros returns 0; (2) Grid with all ones returns 1; (3) A single cell that is land returns 1, water returns 0; (4) Diagonal adjacency means cells touching only corners are still part of the same island. Complexity: Each cell is visited at most once, and for each visited cell we examine 8 neighbors. Thus time is `O(rows * cols * 8)` = `O(rows * cols)`, space is `O(rows * cols)` for the visited matrix plus recursion stack depth up to `rows * cols` in the worst case (e.g., a snake-like land mass). To avoid stack overflow on a 50×50 grid, an explicit stack (BFS queue or DFS stack) is safer, but recursion depth of 2500 is generally acceptable on typical platforms. We'll use recursion for clarity but note the alternative.
#include <vector>
#include <cstddef>

// Count the number of distinct islands in a binary grid using 8-directional connectivity.
// The grid is passed by const reference; no modification is performed.
int countIslands(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }
    
    const std::size_t rows = grid.size();
    const std::size_t cols = grid[0].size();
    
    // Visited matrix, same dimensions as grid, initialized to false.
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    
    // 8-directional movement offsets: right, left, down, up, down-right, down-left, up-right, up-left
    const int dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
    const int dy[8] = {0, 0, 1, -1, 1, -1, -1, 1};
    
    int island_count = 0;
    
    // Recursive DFS helper to flood-fill all connected land cells.
    // Uses a lambda with self-reference via std::function for simplicity.
    // Note: Can also be implemented iteratively with an explicit stack.
    std::function<void(std::size_t, std::size_t)> dfs = [&](std::size_t x, std::size_t y) {
        visited[x][y] = true;
        for (int k = 0; k < 8; ++k) {
            std::size_t nx = static_cast<std::size_t>(static_cast<int>(x) + dx[k]);
            std::size_t ny = static_cast<std::size_t>(static_cast<int>(y) + dy[k]);
            // Check bounds (nx and ny are size_t, but we need to ensure they are within [0, rows-1] and [0, cols-1])
            // Since nx and ny are unsigned, negative values become large numbers, so condition nx < rows and ny < cols works.
            if (nx < rows && ny < cols && !visited[nx][ny] && grid[nx][ny] == 1) {
                dfs(nx, ny);
            }
        }
    };
    
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                ++island_count;
                dfs(i, j);
            }
        }
    }
    
    return island_count;
}
#include <cassert>
#include <vector>

// The solution function is expected to be declared above (or included).
// Declaration for compilation:
int countIslands(const std::vector<std::vector<int>>& grid);

int main() {
    // Empty grid
    std::vector<std::vector<int>> empty;
    assert(countIslands(empty) == 0);

    // Single water cell
    std::vector<std::vector<int>> single_water = {{0}};
    assert(countIslands(single_water) == 0);

    // Single land cell
    std::vector<std::vector<int>> single_land = {{1}};
    assert(countIslands(single_land) == 1);

    // All water 3x3
    std::vector<std::vector<int>> all_water = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    assert(countIslands(all_water) == 0);

    // All land 2x2
    std::vector<std::vector<int>> all_land = {
        {1, 1},
        {1, 1}
    };
    assert(countIslands(all_land) == 1);

    // Diagonal connectivity — four corner ones form one island
    std::vector<std::vector<int>> diagonal = {
        {1, 0, 1},
        {0, 0, 0},
        {1, 0, 1}
    };
    assert(countIslands(diagonal) == 1);

    // Two separate islands — one vertical, one diagonal
    std::vector<std::vector<int>> two_islands = {
        {1, 0, 0, 1},
        {1, 0, 0, 1},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    };
    // Island1: cells (0,0),(1,0) — vertical. Island2: (0,3),(1,3) — vertical. 
    // Wait (0,3) and (1,3) are adjacent vertically, but are they adjacent diagonally to (2,2)? (0,3) is not diagonal to (2,2). 
    // Let's recompute: (1,3) and (2,2) are diagonal? dx=1, dy=-1 => yes, so they connect. So actually all land cells connect via (1,3)-(2,2). 
    // To have two separate islands, we need a clear gap. Use a better test:
    std::vector<std::vector<int>> two_islands_clear = {
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 1, 1}
    };
    // First island: top-left 2 cells (0,0),(0,1). Second island: bottom-right 2 cells (3,2),(3,3). Not touching even diagonally.
    assert(countIslands(two_islands_clear) == 2);

    // Mixed case: one big island and one isolated cell
    std::vector<std::vector<int>> mixed = {
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0},
        {0, 0, 1, 1, 0}
    };
    // Big island: (0,0),(0,1),(1,0) connect diagonally? (0,1) and (1,0) are diagonal (dx=1, dy=-1) yes, so that's one island.
    // (2,2),(2,3),(3,2),(3,3) is another island. (0,4) is isolated — but (0,4) touches (0,3)? (0,3) is water. No connection. So total 3 islands.
    assert(countIslands(mixed) == 3);

    // Stress: 50x50 all ones -> 1 island
    std::vector<std::vector<int>> large(50, std::vector<int>(50, 1));
    assert(countIslands(large) == 1);

    // Stress: 50x50 checkerboard where no two diagonal neighbors are both 1 — each 1 is isolated.
    // Pattern: place 1 at positions where (r+c) even. That gives diagonal adjacency? Actually (r+c even) and (r+1+c+1 even) -> (r+c+2) even, so both even. That means diagonally adjacent cells both have even sum? For (r,c) even, (r+1,c+1) has sum r+c+2, also even, so they are both 1 — thus they connect diagonally. So use a different pattern: place 1 only on rows where r is even and c is even? Then (r,c) and (r+2,c) are not adjacent (gap). (r,c) and (r+1,c+1) has r odd? Not both. Best to just use a simple pattern: only cells where r==c are 1. Then each 1 is isolated because neighbors are not on main diagonal. But (0,0) and (1,1) are diagonal? Yes, dx=1, dy=1, so they connect. So we need spacing. Use only cells where r%3==0 and c%3==0? That's too sparse. Simpler: just test a 3x3 with only center 1.
    std::vector<std::vector<int>> center_only = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    assert(countIslands(center_only) == 1);

    return 0;
}

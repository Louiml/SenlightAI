Write a C++ function `countConnectedRegions(const vector<vector<int>>& grid)` that treats `1` as land and `0` as water, and returns a `pair<int, int>` where the first element is the number of distinct connected regions of land (using 4-directional adjacency: up, down, left, right), and the second element is the size (number of cells) of the largest such region. The grid dimensions can be from 0×0 up to 1000×1000. If there is no land, return `{0, 0}`. The function must not modify the input grid and must handle edge cases like a single cell, a fully water grid, and irregular (non-square) rectangles.

The solution uses a standard breadth‑first search (BFS) traversal across the grid. Iterate through every cell; whenever an unvisited land cell is found, increment the region counter and start a BFS from that cell. During the BFS, use a queue of coordinate pairs to explore all 4‑neighbors. Mark each visited cell immediately to prevent reprocessing and to ensure each cell is counted once. Track the number of cells visited in this BFS as `temp` and update the global maximum after the BFS ends. Important edge cases: empty grid (0 rows or 0 columns) → return `{0,0}`; grid with only zeros → return `{0,0}`; grid with one land cell → return `{1,1}`; ensure bounds checking to avoid out‑of‑range accesses, especially for coordinates near the edges. Time complexity is O(R×C) because each cell is pushed and popped at most once. Space complexity is O(R×C) in the worst case for the visited array and BFS queue (when the entire grid is land).

#include <vector>
#include <queue>
#include <utility>

// Count connected land regions and the largest region size in a binary grid.
// 1 = land, 0 = water. Uses 4-directional adjacency.
std::pair<int, int> countConnectedRegions(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return {0, 0};
    }
    
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    
    int regionCount = 0;
    int maxSize = 0;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!grid[r][c] || visited[r][c]) continue;
            
            // Found a new region, start BFS
            ++regionCount;
            int regionSize = 0;
            std::queue<std::pair<int, int>> q;
            q.push({r, c});
            visited[r][c] = true;
            
            while (!q.empty()) {
                auto [x, y] = q.front();
                q.pop();
                ++regionSize;
                
                for (int way = 0; way < 4; ++way) {
                    int nx = x + dx[way];
                    int ny = y + dy[way];
                    
                    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) continue;
                    if (!grid[nx][ny] || visited[nx][ny]) continue;
                    
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
            
            if (regionSize > maxSize) {
                maxSize = regionSize;
            }
        }
    }
    
    return {regionCount, maxSize};
}

#include <cassert>
#include <vector>
#include <utility>

// forward declaration of the function under test
std::pair<int, int> countConnectedRegions(const std::vector<std::vector<int>>& grid);

int main() {
    // Single land cell
    assert(countConnectedRegions({{1}}) == std::make_pair(1, 1));
    
    // Single water cell
    assert(countConnectedRegions({{0}}) == std::make_pair(0, 0));
    
    // Empty grid (0 rows)
    assert(countConnectedRegions({}) == std::make_pair(0, 0));
    
    // All water 3x3
    assert(countConnectedRegions({{0,0,0},{0,0,0},{0,0,0}}) == std::make_pair(0, 0));
    
    // One big region 3x3 all land
    assert(countConnectedRegions({{1,1,1},{1,1,1},{1,1,1}}) == std::make_pair(1, 9));
    
    // Two separate regions with different sizes
    std::vector<std::vector<int>> grid1 = {
        {1,1,0,0},
        {0,0,0,1},
        {1,1,0,1}
    };
    // Regions: top-left block (3 cells), single cell at (1,3), single cell at (2,3) => largest size = 3
    assert(countConnectedRegions(grid1) == std::make_pair(3, 3));
    
    // Diagonal cells do not touch
    std::vector<std::vector<int>> grid2 = {
        {1,0},
        {0,1}
    };
    assert(countConnectedRegions(grid2) == std::make_pair(2, 1));
    
    // Irregular rectangle (2 rows, 5 columns)
    std::vector<std::vector<int>> grid3 = {
        {1,1,1,0,1},
        {0,0,1,1,0}
    };
    // Regions: big shape on left (cells: (0,0),(0,1),(0,2),(1,2),(1,3) => 5), plus single cell (0,4) => 1
    assert(countConnectedRegions(grid3) == std::make_pair(2, 5));
    
    return 0;
}

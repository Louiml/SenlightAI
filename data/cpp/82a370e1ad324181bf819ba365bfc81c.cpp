Write a C++ function `int countIslands(const std::vector<std::string>& grid)` that takes a rectangular grid of characters where `'L'` represents land, `'W'` represents water, and returns the number of distinct islands. An island is a connected group of land cells where connectivity is defined by 4-directional adjacency (up, down, left, right). The grid will have at least 1 row and 1 column, contain only uppercase `'L'` and `'W'` characters, and may have islands that touch the border. The function must not modify the input and must handle grids up to 150x150 cells efficiently.
The solution uses depth-first search (DFS) to explore each unvisited land cell. The algorithm iterates through every cell in the grid. When it encounters an unvisited `'L'`, it increments the island counter and performs a DFS from that cell, marking all connected land cells as visited. The DFS recursively checks the four orthogonal neighbors, skipping any that are out of bounds, water, or already visited. This ensures each connected component of land is counted exactly once. Edge cases include grids with no land (returns 0), a single land cell, islands that touch the border (handled by bounds checking), and isolated land cells separated by water. Time complexity is O(n*m) because each cell is visited at most once. Space complexity is O(n*m) for the visited array in the worst case (e.g., an all-land grid) due to the recursion stack and the visited matrix.
#include <vector>
#include <string>

// Count the number of distinct islands in a grid where 'L' is land and 'W' is water.
// Connectivity is 4-directional (up, down, left, right).
int countIslands(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    int islandCount = 0;
    
    // Direction vectors for 4-neighbor movement: up, down, left, right
    const int dRow[] = {-1, 1, 0, 0};
    const int dCol[] = {0, 0, -1, 1};
    
    // Recursive DFS to mark all cells of an island
    auto dfs = [&](int r, int c) -> void {
        visited[r][c] = true;
        for (int k = 0; k < 4; ++k) {
            int nr = r + dRow[k];
            int nc = c + dCol[k];
            // Check bounds, land, and not visited
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && 
                grid[nr][nc] == 'L' && !visited[nr][nc]) {
                dfs(nr, nc);
            }
        }
    };
    
    // Iterate over all cells
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'L' && !visited[i][j]) {
                ++islandCount;
                dfs(i, j);
            }
        }
    }
    
    return islandCount;
}
#include <cassert>
#include <vector>
#include <string>

// Solution function is declared above (or included here for completeness)
// Here we assume the function is available.

int main() {
    // Test 1: Single land cell
    assert(countIslands({"L"}) == 1);
    // Test 2: Single water cell
    assert(countIslands({"W"}) == 0);
    // Test 3: 1x4 row with two separate islands
    assert(countIslands({"LWLL"}) == 2);
    // Test 4: 2x2 all land -> one island
    assert(countIslands({"LL", "LL"}) == 1);
    // Test 5: 3x3 with ring around water (one island)
    assert(countIslands({"LLL", "LWL", "LLL"}) == 1);
    // Test 6: 3x3 with diagonal only (two islands)
    assert(countIslands({"LWL", "WLW", "LWL"}) == 4); // Each corner is separate
    // Test 7: Border islands touching edges
    assert(countIslands({"LWW", "WWW", "WWL"}) == 2);
    // Test 8: Empty grid
    assert(countIslands({}) == 0);
    // Test 9: Larger grid with multiple islands (3x5)
    assert(countIslands({"LLWLL", "LWLLW", "LLWLL"}) == 3);
    // Test 10: All water
    assert(countIslands({"WWW", "WWW"}) == 0);
    
    return 0;
}

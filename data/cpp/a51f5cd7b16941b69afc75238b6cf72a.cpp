Write a C++ function `countIslands` that takes a rectangular grid represented as a `std::vector<std::vector<int>>` where 1 represents land and 0 represents water, and returns the number of distinct islands. An island is a group of connected 1s (horizontally or vertically adjacent, but not diagonally). The grid may be empty, may contain rows of different lengths (treat missing cells as water), and may contain all zeros or all ones. The function must be `const`-correct and should not modify the input.

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Empty grid
    assert(countIslands({}) == 0);
    
    // All zeros
    assert(countIslands({{0, 0, 0}, {0, 0, 0}}) == 0);
    
    // All ones (one big island)
    assert(countIslands({{1, 1}, {1, 1}}) == 1);
    
    // Single cell
    assert(countIslands({{1}}) == 1);
    assert(countIslands({{0}}) == 0);
    
    // Simple two islands
    assert(countIslands({{1, 0, 1}, {0, 0, 0}, {1, 0, 1}}) == 4);
    
    // Larger connected components
    assert(countIslands({{1, 1, 0}, {1, 0, 0}, {0, 0, 1}}) == 2);
    
    // Diagonal neighbors are not connected
    assert(countIslands({{1, 0}, {0, 1}}) == 2);
    
    // Irregular row lengths (missing cells treated as water)
    assert(countIslands({{1}, {1, 1}, {}}) == 1);
    
    // Mixed with multiple islands
    assert(countIslands({{1, 0, 1, 0}, {1, 0, 1, 0}, {0, 0, 0, 1}}) == 3);
    
    // Zero rows but non-zero columns (no land)
    assert(countIslands({{}, {}}) == 0);
    
    return 0;
}

#include <vector>

// Count the number of distinct islands (connected groups of 1s) in a grid.
// Connections are horizontal/vertical only. Input is not modified.
int countIslands(const std::vector<std::vector<int>>& grid) {
    if (grid.empty()) return 0;
    
    int rows = static_cast<int>(grid.size());
    int maxCols = 0;
    for (const auto& row : grid) {
        maxCols = std::max(maxCols, static_cast<int>(row.size()));
    }
    if (maxCols == 0) return 0;
    
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(maxCols, false));
    int islandCount = 0;
    
    // Recursive DFS to mark all connected land cells from (r, c).
    auto dfs = [&](int r, int c) -> void {
        if (r < 0 || r >= rows || c < 0 || c >= maxCols) return;
        if (c >= static_cast<int>(grid[r].size())) return; // Treat missing as water
        if (visited[r][c] || grid[r][c] == 0) return;
        visited[r][c] = true;
        dfs(r + 1, c);
        dfs(r - 1, c);
        dfs(r, c + 1);
        dfs(r, c - 1);
    };
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < maxCols; ++c) {
            if (c < static_cast<int>(grid[r].size()) && grid[r][c] == 1 && !visited[r][c]) {
                ++islandCount;
                dfs(r, c);
            }
        }
    }
    
    return islandCount;
}

// The solution uses depth-first search (DFS) to traverse each unvisited land cell and mark all connected land cells as visited. We iterate over every cell in the grid; when we encounter a 1 that has not been visited, we increment the island count and perform a recursive DFS from that cell, visiting all four directional neighbors (up, down, left, right) that are within bounds, contain a 1, and have not been visited. Visited cells are tracked in a separate boolean grid of the same dimensions, or we could modify the input but the task requires no modification, so a separate visited structure is needed. Edge cases: empty grid returns 0; rows of differing lengths are handled by checking bounds per row; all zeros returns 0; all ones returns 1 (since they are all connected). For a grid with R rows and C columns (maximum length), the time complexity is O(R * C) because each cell is visited at most once during DFS, and the space complexity is O(R * C) for the visited array plus the recursive stack depth (worst-case O(R * C) for a full grid of ones).

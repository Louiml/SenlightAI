Write a C++ function that, given a rectangular grid of 0s (water) and 1s (land), returns the total perimeter of all land cells. Each land cell contributes 4 sides, but sides shared with adjacent land cells (horizontally or vertically) are not part of the outer perimeter. The grid can be empty or have zero rows, and the function must handle such cases gracefully by returning 0. Adjacent land cells only share sides if they are directly next to each other; diagonal touches do not count. The input grid is passed as a constant reference, and the function must not modify it.
#include <cassert>
#include <vector>

int main() {
    // Example from the problem
    std::vector<std::vector<int>> g1 = {
        {0,1,0,0},
        {1,1,1,0},
        {1,1,0,0},
        {1,1,0,0}
    };
    assert(islandPerimeter(g1) == 16);

    // Single cell island
    std::vector<std::vector<int>> g2 = {{1}};
    assert(islandPerimeter(g2) == 4);

    // Single row, multiple islands separated by water
    std::vector<std::vector<int>> g3 = {{1,0,1}};
    assert(islandPerimeter(g3) == 8); // two separate cells

    // All water
    std::vector<std::vector<int>> g4 = {{0,0},{0,0}};
    assert(islandPerimeter(g4) == 0);

    // Empty grid
    std::vector<std::vector<int>> g5;
    assert(islandPerimeter(g5) == 0);

    // Grid with zero columns
    std::vector<std::vector<int>> g6 = {{}, {}};
    assert(islandPerimeter(g6) == 0);

    // Full 2x2 block
    std::vector<std::vector<int>> g7 = {{1,1},{1,1}};
    assert(islandPerimeter(g7) == 8); // outer perimeter of 2x2 square

    // L-shaped island
    std::vector<std::vector<int>> g8 = {
        {1,0},
        {1,1}
    };
    assert(islandPerimeter(g8) == 8);

    // Large single row all land
    std::vector<std::vector<int>> g9 = {{1,1,1,1}};
    assert(islandPerimeter(g9) == 10); // 4*4 - 2*3 = 10

    // Two separate islands in different rows
    std::vector<std::vector<int>> g10 = {
        {1,0},
        {0,1}
    };
    assert(islandPerimeter(g10) == 8);

    return 0;
}
#include <vector>

// Returns the perimeter of the island in a rectangular grid.
// grid[i][j] == 1 means land, 0 means water.
// Out-of-bounds cells are treated as water.
int islandPerimeter(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    
    int rows = grid.size();
    int cols = grid[0].size();
    int perimeter = 0;
    
    // Directions: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 1) {
                // Check four neighbors
                for (int k = 0; k < 4; ++k) {
                    int nr = r + dr[k];
                    int nc = c + dc[k];
                    // Count as perimeter if neighbor is out of bounds or water
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || grid[nr][nc] == 0) {
                        ++perimeter;
                    }
                }
            }
        }
    }
    
    return perimeter;
}
// The solution iterates through every cell in the grid. For each land cell (value == 1), we count how many of its four orthogonal neighbors (up, down, left, right) are either out of bounds or water (value == 0). Each such neighbor contributes 1 to the perimeter of that cell. Summing these contributions across all land cells gives the total perimeter. Edge cases include empty grids (0 rows or 0 columns), which yield 0, and single-cell islands, which have perimeter 4 (all four neighbors are out of bounds or water). The algorithm runs in O(rows × columns) time, as it visits each cell exactly once, and uses O(1) extra space beyond the input. The neighbor check is done via a helper that treats out-of-bounds coordinates as water, simplifying boundary handling.

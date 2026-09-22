Write a standalone C++ function named `countIslands` that takes a 2D vector of characters (`std::vector<std::vector<char>>`) representing a grid where `'1'` indicates land and `'0'` indicates water. The function must return the number of distinct islands, where an island is a group of adjacent `'1'` cells connected horizontally or vertically (not diagonally). The function must treat the input grid as read-only (do not modify it), so it should internally create a copy if mutation is needed for traversal. The grid dimensions can be zero (empty grid), in which case the result is `0`. The grid may contain only `'1'` and `'0'` characters.
// The problem is a classic flood-fill / connected components problem. The main algorithm scans every cell in the grid. When a `'1'` is encountered, it increments the island count and performs a depth-first search (DFS) from that cell to mark all connected land cells as visited. To avoid modifying the original grid, we either make a mutable copy or use a separate visited matrix. The DFS recursively explores the four orthogonal neighbors (up, down, left, right), and marks the current cell as visited (e.g., by setting it to `'0'` in the copy). Edge cases include: an empty grid (no rows or no columns) should return `0`; a grid with only water returns `0`; a grid with one isolated `'1'` returns `1`; and a grid with fully connected land returns `1`. Time complexity is \(O(R \times C)\) because each cell is visited at most once during the flood fill, and space complexity is \(O(R \times C)\) in the worst case for the recursion stack (for a grid that is all land) plus the copy of the grid, so overall \(O(R \times C)\).
#include <vector>

// Count the number of distinct islands (groups of adjacent '1's) in a grid.
// The input grid is not modified; a mutable copy is used for traversal.
int countIslands(const std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    int rows = grid.size();
    int cols = grid[0].size();

    // Work on a mutable copy so the original grid remains unchanged.
    std::vector<std::vector<char>> copy = grid;

    // Depth-first search to mark all connected land cells as visited.
    // The lambda captures by reference to avoid passing many parameters.
    auto dfs = [&](int r, int c, auto&& dfs_ref) -> void {
        if (r < 0 || r >= rows || c < 0 || c >= cols || copy[r][c] != '1') {
            return;
        }
        copy[r][c] = '0';  // Mark as visited
        dfs_ref(r + 1, c, dfs_ref);
        dfs_ref(r - 1, c, dfs_ref);
        dfs_ref(r, c + 1, dfs_ref);
        dfs_ref(r, c - 1, dfs_ref);
    };

    int island_count = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (copy[r][c] == '1') {
                ++island_count;
                dfs(r, c, dfs);
            }
        }
    }

    return island_count;
}
#include <cassert>
#include <vector>

// The solution function is declared above (countIslands).

int main() {
    // Test 1: Simple grid with two separate islands.
    std::vector<std::vector<char>> grid1 = {
        {'1','1','0','0'},
        {'1','0','0','0'},
        {'0','0','1','1'},
        {'0','0','1','1'}
    };
    assert(countIslands(grid1) == 2);

    // Test 2: All water.
    std::vector<std::vector<char>> grid2 = {
        {'0','0','0'},
        {'0','0','0'}
    };
    assert(countIslands(grid2) == 0);

    // Test 3: All land (one island).
    std::vector<std::vector<char>> grid3 = {
        {'1','1'},
        {'1','1'}
    };
    assert(countIslands(grid3) == 1);

    // Test 4: Single isolated land.
    std::vector<std::vector<char>> grid4 = {
        {'0','1','0'},
        {'0','0','0'}
    };
    assert(countIslands(grid4) == 1);

    // Test 5: Empty grid.
    std::vector<std::vector<char>> grid5;
    assert(countIslands(grid5) == 0);

    // Test 6: One row.
    std::vector<std::vector<char>> grid6 = {
        {'1','0','1','0','1'}
    };
    assert(countIslands(grid6) == 3);

    // Test 7: One column.
    std::vector<std::vector<char>> grid7 = {
        {'1'}, {'0'}, {'1'}, {'1'}
    };
    assert(countIslands(grid7) == 2);

    // Test 8: Ensure the input grid is not modified.
    std::vector<std::vector<char>> grid8 = {
        {'1','1'},
        {'1','0'}
    };
    std::vector<std::vector<char>> original = grid8;
    countIslands(grid8);
    assert(grid8 == original);

    // Test 9: Large connected shape with holes.
    std::vector<std::vector<char>> grid9 = {
        {'1','0','1'},
        {'1','0','1'},
        {'1','1','1'}
    };
    assert(countIslands(grid9) == 2); // Left vertical and right vertical are disconnected due to middle column of zeros.

    // Test 10: Grid with many isolated lands.
    std::vector<std::vector<char>> grid10 = {
        {'1','0','1'},
        {'0','0','0'},
        {'1','0','1'}
    };
    assert(countIslands(grid10) == 4);

    return 0;
}

Write a standalone C++ function named `countConnectedLand` that takes a `const std::vector<std::vector<char>>&` representing a grid where `'1'` indicates land and `'0'` indicates water, and returns an `int` equal to the number of connected land components (islands). Two land cells are considered connected if they are adjacent horizontally or vertically (not diagonally). The grid may be empty or contain rows of varying lengths; if any row is empty or the grid is empty, return 0. The function must not modify the input grid. Ensure your solution uses depth‑first search (DFS) with an auxiliary visited structure, not by mutating the original grid.

// The core idea is to scan every cell of the grid. Whenever an unvisited land cell (`'1'`) is encountered, we increment the island count and perform a depth‑first search (DFS) that recurses into all four cardinal neighbors (up, down, left, right) to mark every connected land cell as visited. We use a separate `std::vector<std::vector<bool>>` visited grid to avoid modifying the input, since the input is passed as `const`. Edge cases include an empty grid, an empty row, or rows with different lengths – the method should handle rectangular or irregular grids by checking bounds each recursion. Time complexity is O(R×C) where R is the number of rows and C the maximum row length, because each cell is visited at most once. Space complexity is also O(R×C) for the visited grid, plus recursion stack depth up to O(R×C) in the worst case of a fully land-filled grid.

#include <vector>
#include <functional>

int countConnectedLand(const std::vector<std::vector<char>>& grid) {
    if (grid.empty()) return 0;
    const int rows = static_cast<int>(grid.size());
    int cols = 0;
    for (const auto& row : grid) {
        if (static_cast<int>(row.size()) > cols) {
            cols = static_cast<int>(row.size());
        }
    }
    if (cols == 0) return 0;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    int islandCount = 0;

    std::function<void(int, int)> dfs = [&](int r, int c) {
        // Bounds check and water check
        if (r < 0 || r >= rows || c < 0 || c >= cols) return;
        if (visited[r][c] || grid[r][c] != '1') return;
        visited[r][c] = true;
        dfs(r - 1, c);
        dfs(r + 1, c);
        dfs(r, c - 1);
        dfs(r, c + 1);
    };

    for (int r = 0; r < rows; ++r) {
        // Only iterate up to actual row length
        int actualCols = static_cast<int>(grid[r].size());
        for (int c = 0; c < actualCols; ++c) {
            if (!visited[r][c] && grid[r][c] == '1') {
                ++islandCount;
                dfs(r, c);
            }
        }
    }

    return islandCount;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic 4x5 grid with 1 island
    std::vector<std::vector<char>> grid1 = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };
    assert(countConnectedLand(grid1) == 3);

    // Test 2: Empty grid
    std::vector<std::vector<char>> grid2;
    assert(countConnectedLand(grid2) == 0);

    // Test 3: Empty row (irregular)
    std::vector<std::vector<char>> grid3 = {
        {'1','0','1'},
        {},
        {'0','1','0'}
    };
    assert(countConnectedLand(grid3) == 3);

    // Test 4: All water
    std::vector<std::vector<char>> grid4 = {
        {'0','0'},
        {'0','0'}
    };
    assert(countConnectedLand(grid4) == 0);

    // Test 5: All land (single island)
    std::vector<std::vector<char>> grid5 = {
        {'1','1'},
        {'1','1'}
    };
    assert(countConnectedLand(grid5) == 1);

    // Test 6: Single cell land
    std::vector<std::vector<char>> grid6 = {{'1'}};
    assert(countConnectedLand(grid6) == 1);

    // Test 7: Diagonal connectivity is not counted
    std::vector<std::vector<char>> grid7 = {
        {'1','0'},
        {'0','1'}
    };
    assert(countConnectedLand(grid7) == 2);

    // Test 8: Row lengths differ, land at edges
    std::vector<std::vector<char>> grid8 = {
        {'1'},
        {'1','1'},
        {'1','0','1'}
    };
    assert(countConnectedLand(grid8) == 2); // left column connected, right cell separate

    // Test 9: Verify input not modified
    std::vector<std::vector<char>> grid9 = {
        {'1','1'},
        {'1','0'}
    };
    std::vector<std::vector<char>> original = grid9;
    countConnectedLand(grid9);
    assert(grid9 == original);

    return 0;
}

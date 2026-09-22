// Write a C++ function `countClosedIslands(const std::vector<std::vector<int>>& grid)` that returns the number of "closed islands" in a binary matrix (0 = water, 1 = land). A closed island is a group of connected 0s (via 4-directional adjacency) that is completely surrounded by 1s (land) on all sides, including diagonals not being considered — but crucially, any 0-group that touches the border of the matrix is **not** closed. The input matrix is non-empty and rectangular. The function must be `const`-correct and avoid modifying the input.
// The main idea is to mark all water cells (0s) that are connected to the matrix border, because these cells can never belong to a closed island. We do this via DFS starting from every border cell that contains a 0. After marking, any remaining unvisited 0-cell must belong to a closed island. We then iterate through all cells; every time we find an unvisited 0, we increment the island count and perform DFS to mark all cells of that closed island as visited, so they aren’t counted again. Edge cases: an empty grid is not expected (but can be handled with a zero check), a grid with no 0s returns 0, and a grid where all 0s touch the border returns 0. Time complexity is \(O(n \times m)\) because each cell is visited at most twice (once during border DFS, once during island counting). Space complexity is \(O(n \times m)\) for the visited matrix and the recursion stack (worst case \(O(n \times m)\) for a fully connected water region).
#include <vector>

// Counts the number of closed islands in a binary matrix.
// 0 = water, 1 = land. A closed island is a group of 0s
// that is completely surrounded by 1s and does not touch the border.
int countClosedIslands(const std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    if (n == 0) return 0;
    int m = grid[0].size();
    if (m == 0) return 0;

    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));

    // Lambda for DFS traversal
    auto dfs = [&](int x, int y, auto&& dfs_ref) -> void {
        if (x < 0 || y < 0 || x >= n || y >= m || visited[x][y] || grid[x][y] != 0)
            return;
        visited[x][y] = true;
        dfs_ref(x + 1, y, dfs_ref);
        dfs_ref(x, y + 1, dfs_ref);
        dfs_ref(x - 1, y, dfs_ref);
        dfs_ref(x, y - 1, dfs_ref);
    };

    // Mark all water cells connected to the border
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            bool isBorder = (i == 0 || j == 0 || i == n - 1 || j == m - 1);
            if (isBorder && grid[i][j] == 0 && !visited[i][j]) {
                dfs(i, j, dfs);
            }
        }
    }

    int count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 0 && !visited[i][j]) {
                ++count;
                dfs(i, j, dfs);
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>

// (Solution code included here for testing, but in a real setup,
// the function would be declared in a header.)

int countClosedIslands(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Provided example
    std::vector<std::vector<int>> grid1 = {
        {1,1,1,1,1,1,1,0},
        {1,0,0,0,0,1,1,0},
        {1,0,1,0,1,1,1,0},
        {1,0,0,0,0,1,0,1},
        {1,1,1,1,1,1,1,0}
    };
    assert(countClosedIslands(grid1) == 1);

    // Test 2: No water, no closed islands
    std::vector<std::vector<int>> grid2 = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };
    assert(countClosedIslands(grid2) == 0);

    // Test 3: Single water cell fully surrounded (3x3 with 0 in middle)
    std::vector<std::vector<int>> grid3 = {
        {1,1,1},
        {1,0,1},
        {1,1,1}
    };
    assert(countClosedIslands(grid3) == 1);

    // Test 4: Water touching border is not closed
    std::vector<std::vector<int>> grid4 = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    assert(countClosedIslands(grid4) == 0);

    // Test 5: Multiple closed islands
    std::vector<std::vector<int>> grid5 = {
        {1,1,1,1,1},
        {1,0,1,0,1},
        {1,1,1,1,1}
    };
    assert(countClosedIslands(grid5) == 2);

    // Test 6: Large closed region
    std::vector<std::vector<int>> grid6 = {
        {1,1,1,1},
        {1,0,0,1},
        {1,0,0,1},
        {1,1,1,1}
    };
    assert(countClosedIslands(grid6) == 1);

    // Test 7: All water, but touches border -> 0
    std::vector<std::vector<int>> grid7 = {
        {0,0},
        {0,0}
    };
    assert(countClosedIslands(grid7) == 0);

    // Test 8: Single row, no closed islands
    std::vector<std::vector<int>> grid8 = { {0,1,0} };
    assert(countClosedIslands(grid8) == 0);

    return 0;
}
